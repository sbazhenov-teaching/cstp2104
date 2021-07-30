#pragma once
#include <networkLib/winSockInit.h>
#include <networkLib/socket.h>
#include <windowLib/window.h>
#include <windowLib/handle.h>
#include <functional>
#include <windowLib/keys.h>

namespace ServerApp
{

class Server
{

enum class State
{
    Command,
    Value
};

public:
    Server(HINSTANCE hInstance);
    ~Server();

    using KeyType = unsigned short;
    using KeyCallback = std::function<void(KeyType)>;
    using StepCallback = std::function<void(unsigned)>;

    void init(KeyCallback, StepCallback);
    Network::Socket acceptClient();
    void serve();
    void stop();
private:
    State mState{State::Command};
    Command mExpectedValue;
    Network::WinSock mWinSock;
    Network::Socket mSockets[2];
    //Window mWindow;
    Handle mStopEvent;
    WSAEVENT mListenEvent;
    KeyCallback mKeyCallback;
    StepCallback mStepCallback;

    void onCreate(Window&);
    LRESULT processMessage(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

}