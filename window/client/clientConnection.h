#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>
#include <windowLib/keys.h>

class ClientConnection
{
public:
    void init();
    void sendToServer(Key);
    void shutdown();
private:
    Network::WinSock mWinSock;
    Network::Socket mConnectSocket;
};


