#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networkLib/addrInfo.h>
#include <assert.h>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#define WM_WSAASYNC (WM_USER +5)

namespace ServerApp
{

const wchar_t* cWndClassName{ L"NetworkWindow" };

Server::Server(HINSTANCE hInstance)
//    : mWindow{
//        cWndClassName,
//        (Window::registerClass(cWndClassName), hInstance),
//        [this](Window& w) { onCreate(w); },
//        [this](Window& w, UINT message, WPARAM wParam, LPARAM lParam) { return processMessage(w.getHwnd(), message, wParam, lParam); },
//        L"Network"
//}
{
    mStopEvent = ::CreateEvent(nullptr, FALSE, FALSE, L"NetworkStopEvent");
}

Server::~Server()
{
    ::WSACloseEvent(mListenEvent);
    ::CloseHandle(mStopEvent);
}

void Server::init()
{
    {
        {
            struct addrinfo hints;
            ::ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = AI_PASSIVE;

            Network::AddrInfo addrInfo{ nullptr, DEFAULT_PORT, &hints };
            // Create a SOCKET for connecting to server
            mListenSocket = Network::Socket{ addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol };
            if (mListenSocket == INVALID_SOCKET) {
                return;
            }

            //::WSAAsyncSelect(mListenSocket, mWindow.getHwnd(), WM_WSAASYNC, FD_READ | FD_WRITE |
            //    FD_ACCEPT | FD_CLOSE);

            // Setup the TCP listening socket
            {
                int iResult = ::bind(mListenSocket, addrInfo->ai_addr, (int)addrInfo->ai_addrlen);
                assert(iResult != SOCKET_ERROR);
            }
        }

        mListenEvent = ::WSACreateEvent();
        int selectResult{ WSAEventSelect(mListenSocket, mListenEvent, FD_ACCEPT | FD_CLOSE) };
        assert(selectResult != SOCKET_ERROR);

        {
            int iResult = ::listen(mListenSocket, SOMAXCONN);
            assert(iResult != SOCKET_ERROR);
        }
    }
}

Network::Socket Server::acceptClient()
{
    Network::Socket clientSocket{ ::accept(mListenSocket, NULL, NULL) };
    assert(clientSocket.isValid());
    return clientSocket;
}

void Server::serve()
{
    int iResult;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    WSAEVENT allEvents[2];
    allEvents[0] = mListenEvent;
    allEvents[1] = mStopEvent;
    WSANETWORKEVENTS networkEvents;
    bool stopping{ false };
    Network::Socket clientSocket;
    while (!stopping)
    {
        DWORD waitResult{ ::WSAWaitForMultipleEvents(2, allEvents, FALSE, WSA_INFINITE, FALSE) };
        switch (waitResult)
        {
        case WSA_WAIT_EVENT_0:
        {
            int enumResult{ ::WSAEnumNetworkEvents(mListenSocket,
                allEvents[waitResult - WSA_WAIT_EVENT_0], &networkEvents) };
            assert(enumResult != SOCKET_ERROR);
            if (networkEvents.lNetworkEvents & FD_ACCEPT)
            {
                assert(networkEvents.iErrorCode[FD_ACCEPT_BIT] == 0);
                clientSocket = acceptClient();
            }
            break;
        }
        case WSA_WAIT_EVENT_0 + 1:
            stopping = true;
            break;
        }
    }

    /*
    Network::Socket clientSocket{ acceptClient() };

    // Receive until the peer shuts down the connection
    do {

        iResult = ::recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            //printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = ::send(clientSocket, recvbuf, iResult, 0);
            assert(iSendResult != SOCKET_ERROR);
            //printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            ;//printf("Connection closing...\n");
        else {
            return;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = ::shutdown(clientSocket, SD_SEND);
    assert(iResult != SOCKET_ERROR);
    */
}

void Server::stop()
{
    ::SetEvent(mStopEvent);
}

void Server::onCreate(Window&)
{
}

LRESULT Server::processMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}

}