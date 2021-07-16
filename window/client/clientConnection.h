#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>

class ClientConnection
{
public:
    void init();
    void sendToServer();
    void shutdown();
private:
    Network::WinSock mWinSock;
    Network::Socket mConnectSocket;
};


