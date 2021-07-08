#include "addrInfo.h"
#include <assert.h>
#include <ws2tcpip.h>

namespace Network
{

AddrInfo::AddrInfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints)
{
    int result = ::getaddrinfo(pNodeName, pServiceName, pHints, &mResult);
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

AddrInfo::operator addrinfo* () const
{
    return mResult;
}

}
