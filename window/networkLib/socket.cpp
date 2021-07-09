#include "socket.h"
#include <algorithm>

namespace Network
{

Socket::Socket(SOCKET socket)
    : mSocket{ socket }
{
}

Socket& Socket::operator=(SOCKET socket)
{
    if (mSocket != INVALID_SOCKET)
        ::closesocket(mSocket);
    mSocket = socket;
    return *this;
}

Socket::Socket(int af, int type, int protocol)
    : mSocket{::socket(af, type, protocol)}
{
}

Socket::Socket(Socket&& arg)
    : mSocket(arg.mSocket)
{
    arg.mSocket = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& arg)
{
    std::swap(arg.mSocket, mSocket);
    return *this;
}

Socket::operator SOCKET()
{
    return mSocket;
}

bool Socket::isValid() const
{
    return mSocket != INVALID_SOCKET;
}

Socket::~Socket()
{
    if (mSocket != INVALID_SOCKET)
        ::closesocket(mSocket);
}

}