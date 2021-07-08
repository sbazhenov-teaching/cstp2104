#include "addrInfo.h"
#include <assert.h>
#include <ws2tcpip.h>

namespace Network
{

AddrInfo::AddrInfo(PCSTR pServiceName, const ADDRINFOA* pHints)
{
    int result = ::getaddrinfo(nullptr, pServiceName, pHints, &mResult);
    assert(result == 0);
}

AddrInfo::~AddrInfo()
{
    ::freeaddrinfo(mResult);
}

addrinfo* AddrInfo::operator->() const
{
    return mResult;
}

}
