#pragma once
#include <winsock2.h>

namespace Network
{

class Socket
{
public:
    Socket() = default;
    Socket(SOCKET);
    Socket& operator = (SOCKET);
    Socket(int, int, int);
    Socket(const Socket&) = delete;
    Socket& operator =(const Socket&) = delete;
    Socket(Socket&&);
    Socket& operator =(Socket&&);

    operator SOCKET();
    bool isValid() const;

    ~Socket();
private:
    SOCKET mSocket{ INVALID_SOCKET };
};

}
