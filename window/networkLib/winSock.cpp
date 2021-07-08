#include "winSock.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>

namespace Network
{

WinSock::WinSock()
{
    WSADATA wsaData;
    int result{ ::WSAStartup(MAKEWORD(2, 2), &wsaData) };
    assert(result == 0);
}

WinSock::~WinSock()
{
    ::WSACleanup();
}

}