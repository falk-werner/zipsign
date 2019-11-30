#ifndef OPENSSL_BIGNUMBER_HPP
#define OPENSSL_BIGNUMBER_HPP

#include <openssl/bn.h>

namespace openssl
{

class BigNumber
{
public:
    BigNumber();
    ~BigNumber();
    BigNumber(BigNumber const & other);
    BigNumber(BigNumber && other);
    BigNumber operator=(BigNumber const & other);
    BigNumber operator=(BigNumber && other);
    operator BIGNUM * ();
    void setWord(BN_ULONG value_);
private:
    BIGNUM * value;
};

}

#endif
