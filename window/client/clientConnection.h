#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>
#include <windowLib/keys.h>
#include <assert.h>
#include <string>

class ClientConnection
{
public:
    void init();

    template<typename T>
    void sendToServer(const T& arg)
    {
        // unsigned ints[2];
        // const char* sendbuf = "{value: 43453, asdfasdf: 345346}";
        int iResult{::send(mConnectSocket, reinterpret_cast<const char *>(&arg), sizeof(arg), 0)};
        // int iResult{ ::send(mConnectSocket, reinterpret_cast<const char*>(ints), sizeof(ints), 0) };
        if (iResult == SOCKET_ERROR)
        {
            int err{::WSAGetLastError()};
            ::OutputDebugString(std::to_wstring(err).c_str());
        }
        assert(iResult != SOCKET_ERROR);
    }

    void shutdown();
private:
    Network::WinSock mWinSock;
    Network::Socket mConnectSocket;
};


