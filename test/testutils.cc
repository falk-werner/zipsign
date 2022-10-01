#include "testutils.hpp"

#include <fstream>

namespace testutils
{

void copy_file(std::string const & from, std::string const & to)
{
    std::ifstream source_file(from, std::ios::binary);
    std::ofstream target_file(to, std::ios::binary);

    target_file << source_file.rdbuf();
}

}