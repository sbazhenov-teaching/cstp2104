#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>
#include <windowLib/window.h>
#include <windowLib/handle.h>
#include <functional>

namespace ServerApp
{

class Server
{
public:
    Server(HINSTANCE hInstance);
    ~Server();

    using ReceiveCallback = std::function<void(unsigned)>;

    void init(ReceiveCallback);
    Network::Socket acceptClient();
    void serve();
    void stop();
private:
    Network::WinSock mWinSock;
    Network::Socket mSockets[2];
    //Window mWindow;
    Handle mStopEvent;
    WSAEVENT mListenEvent;
    ReceiveCallback mReceiveCallback;

    void onCreate(Window&);
    LRESULT processMessage(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

}