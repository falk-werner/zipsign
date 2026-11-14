use crate::error::Error;
use std::fs::{File, OpenOptions};
use std::io::{Read, Seek, SeekFrom, Write};
use std::path::{Path, PathBuf};

const MAX_COMMENT_SIZE: usize = 64 * 1024;
const MIN_EOCD_SIZE: usize = 22;
const EOCD_COMMENT_POS: usize = 20;
const MAX_EOCD_SIZE: usize = MIN_EOCD_SIZE + MAX_COMMENT_SIZE;

pub struct Zip {
    path: PathBuf,
}

impl Zip {
    pub fn new<P: AsRef<Path>>(path: P) -> Self {
        Self {
            path: path.as_ref().to_path_buf(),
        }
    }

    fn open_read(&self) -> Result<File, Error> {
        File::open(&self.path).map_err(|e| {
            if e.kind() == std::io::ErrorKind::NotFound {
                Error::FileNotFound(self.path.display().to_string())
            } else {
                Error::Zip(format!("failed to open file {}: {}", self.path.display(), e))
            }
        })
    }

    pub fn get_comment_start(&self) -> Result<u64, Error> {
        let mut file = self.open_read()?;
        let length = file
            .seek(SeekFrom::End(0))
            .map_err(|e| Error::Zip(format!("fseek failed: {}", e)))?;

        if length < MIN_EOCD_SIZE as u64 {
            return Err(Error::Zip("invalid zip archive (too small)".to_string()));
        }

        let buffer_size = std::cmp::min(length as usize, MAX_EOCD_SIZE);
        let offset = length - buffer_size as u64;

        file.seek(SeekFrom::Start(offset))
            .map_err(|e| Error::Zip(format!("fseek failed: {}", e)))?;

        let mut buffer = vec![0u8; buffer_size];
        file.read_exact(&mut buffer)
            .map_err(|e| Error::Zip(format!("fread failed: {}", e)))?;

        let mut found = false;
        let mut pos = buffer_size - MIN_EOCD_SIZE + 1;
        while !found && pos > 0 {
            pos -= 1;
            if buffer[pos] == 0x50
                && buffer[pos + 1] == 0x4b
                && buffer[pos + 2] == 0x05
                && buffer[pos + 3] == 0x06
            {
                found = true;
            }
        }

        if !found {
            return Err(Error::Zip("invalid zip archive: EOCD not found".to_string()));
        }

        Ok(offset + pos as u64 + EOCD_COMMENT_POS as u64)
    }

    pub fn get_comment(&self) -> Result<String, Error> {
        let comment_pos = self.get_comment_start()?;
        let mut file = self.open_read()?;

        let file_size = file
            .seek(SeekFrom::End(0))
            .map_err(|e| Error::Zip(format!("fseek failed: {}", e)))?;

        file.seek(SeekFrom::Start(comment_pos))
            .map_err(|e| Error::Zip(format!("fseek failed: {}", e)))?;

        let mut len_buf = [0u8; 2];
        file.read_exact(&mut len_buf)
            .map_err(|e| Error::Zip(format!("fread failed: {}", e)))?;

        let comment_length = u16::from_le_bytes(len_buf) as usize;

        if comment_pos + 2 + comment_length as u64 > file_size {
            return Err(Error::Zip("comment size exceeds file size".to_string()));
        }

        if comment_length > 0 {
            let mut comment_buf = vec![0u8; comment_length];
            file.read_exact(&mut comment_buf)
                .map_err(|e| Error::Zip(format!("fread failed: {}", e)))?;
            Ok(String::from_utf8_lossy(&comment_buf).into_owned())
        } else {
            Ok(String::new())
        }
    }

    pub fn set_comment(&self, comment: &str) -> Result<(), Error> {
        if comment.len() > MAX_COMMENT_SIZE {
            return Err(Error::Zip("zip comment too long".to_string()));
        }

        let comment_start = self.get_comment_start()?;

        let mut file = OpenOptions::new()
            .read(true)
            .write(true)
            .open(&self.path)
            .map_err(|e| {
                if e.kind() == std::io::ErrorKind::NotFound {
                    Error::FileNotFound(self.path.display().to_string())
                } else {
                    Error::Zip(format!("failed to open file {}: {}", self.path.display(), e))
                }
            })?;

        file.seek(SeekFrom::Start(comment_start))
            .map_err(|e| Error::Zip(format!("fseek failed: {}", e)))?;

        let len_buf = (comment.len() as u16).to_le_bytes();
        file.write_all(&len_buf)
            .map_err(|e| Error::Zip(format!("fwrite failed: {}", e)))?;
        file.write_all(comment.as_bytes())
            .map_err(|e| Error::Zip(format!("fwrite failed: {}", e)))?;

        let new_size = comment_start + 2 + comment.len() as u64;
        file.set_len(new_size)
            .map_err(|e| Error::Zip(format!("ftruncate failed: {}", e)))?;

        Ok(())
    }

    pub fn read_data_to_sign(&self) -> Result<Vec<u8>, Error> {
        let comment_start = self.get_comment_start()?;
        let mut file = self.open_read()?;
        let mut data = vec![0u8; comment_start as usize];
        file.read_exact(&mut data)
            .map_err(|e| Error::Zip(format!("failed to read zip content: {}", e)))?;
        Ok(data)
    }
}
