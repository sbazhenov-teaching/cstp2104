#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>
#include <windowLib/window.h>

namespace ServerApp
{

class Server
{
public:
    Server(HINSTANCE hInstance);
    ~Server();
    void init();
    Network::Socket acceptClient();
    void serve();
    void stop();
private:
    Network::WinSock mWinSock;
    Network::Socket mListenSocket;
    //Window mWindow;
    HANDLE mStopEvent;
    WSAEVENT mListenEvent;

    void onCreate(Window&);
    LRESULT processMessage(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

}