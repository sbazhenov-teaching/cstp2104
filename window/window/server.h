#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>

namespace ServerApp
{

class Server
{
public:
    void init();
    Network::Socket acceptClient();
    void serve();
private:
    Network::WinSock mWinSock;
    Network::Socket mListenSocket;
};

}