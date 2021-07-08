#pragma once
#include <winsock2.h>

namespace Network
{

class AddrInfo
{
public:
    AddrInfo(PCSTR pNodeName, PCSTR pServiceName, const ADDRINFOA* pHints);
    ~AddrInfo();

    addrinfo* operator ->() const;
    operator addrinfo* () const;
private:
    addrinfo* mResult{ nullptr };
};

}
