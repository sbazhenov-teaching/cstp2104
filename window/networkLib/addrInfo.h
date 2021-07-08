#pragma once
#include <winsock2.h>

namespace Network
{

class AddrInfo
{
public:
    AddrInfo(PCSTR pServiceName, const ADDRINFOA* pHints);
    ~AddrInfo();

    addrinfo* operator ->() const;
private:
    addrinfo* mResult{ nullptr };
};

}
