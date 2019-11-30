#include "openssl++/big_number.hpp"
#include "openssl++/exception.hpp"

namespace openssl
{

BigNumber::BigNumber()
{
    value = BN_new();
    if (nullptr == value)
    {
        throw OpenSSLException("failed to create BigNumber");
    }
}

BigNumber::~BigNumber()
{
    BN_free(value);
}

BigNumber::BigNumber(BigNumber const & other)
{
    value = BN_dup(other.value);
    if (nullptr == value)
    {
        throw OpenSSLException("failed to duplicate BigNumber");
    }
}

BigNumber::BigNumber(BigNumber && other)
{
    this->value = other.value;
    other.value = nullptr;
}

BigNumber BigNumber::operator=(BigNumber const & other)
{
    BIGNUM * result = BN_copy(other.value, value);
    if (nullptr == result)
    {
        throw OpenSSLException("failed to copy BigNumber");
    }

    return *this;
}

BigNumber BigNumber::operator=(BigNumber && other)
{
    BN_free(this->value);
    this->value = other.value;
    other.value = nullptr;

    return *this;
}

BigNumber::operator BIGNUM * ()
{
    return value;
}

void BigNumber::setWord(BN_ULONG value_)
{
    int rc = BN_set_word(value, value_);
    if (1 != rc)
    {
        throw new OpenSSLException("failed to add word");
    }
}
   
}