#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networkLib/addrInfo.h>
#include <assert.h>
#include <windowLib/keys.h>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

#define WM_WSAASYNC (WM_USER +5)

namespace ServerApp
{

const wchar_t* cWndClassName{ L"NetworkWindow" };
const unsigned cBufSize{ 0x100 };

Server::Server(HINSTANCE hInstance)
{
    mStopEvent = Handle{ ::CreateEvent(nullptr, FALSE, FALSE, L"NetworkStopEvent") };
}

Server::~Server()
{
    ::WSACloseEvent(mListenEvent);
}

void Server::init(KeyCallback keyCallback, StepCallback stepCallback)
{
    mKeyCallback = std::move(keyCallback);
    mStepCallback = std::move(stepCallback);
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
            mSockets[0] = Network::Socket{ addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol };
            if (mSockets[0] == INVALID_SOCKET) {
                return;
            }

            //::WSAAsyncSelect(mListenSocket, mWindow.getHwnd(), WM_WSAASYNC, FD_READ | FD_WRITE |
            //    FD_ACCEPT | FD_CLOSE);

            // Setup the TCP listening socket
            {
                int iResult = ::bind(mSockets[0], addrInfo->ai_addr, (int)addrInfo->ai_addrlen);
                assert(iResult != SOCKET_ERROR);
            }
        }

        mListenEvent = ::WSACreateEvent();
        assert(WSA_INVALID_EVENT != mListenEvent);
        int selectResult{ ::WSAEventSelect(mSockets[0], mListenEvent, FD_ACCEPT | FD_CLOSE) };
        assert(selectResult != SOCKET_ERROR);

        {
            int iResult = ::listen(mSockets[0], SOMAXCONN);
            assert(iResult != SOCKET_ERROR);
        }
    }
}

Network::Socket Server::acceptClient()
{
    Network::Socket clientSocket{ ::accept(mSockets[0], NULL, NULL) };
    assert(clientSocket.isValid());
    return clientSocket;
}

void Server::serve()
{
    int iResult;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    WSAEVENT allEvents[3];
    allEvents[0] = mStopEvent;
    allEvents[1] = mListenEvent;
    allEvents[2] = WSA_INVALID_EVENT;
    WSANETWORKEVENTS networkEvents;
    int eventNum{ 2 };

    //std::vector<char> buf(cBufSize);
    char buf[cBufSize];
    DWORD offset{0};
    WSABUF bufStruct;
    bufStruct.buf = buf;
    bufStruct.len = cBufSize;
    while (true)
    {
        DWORD waitResult{ ::WSAWaitForMultipleEvents(eventNum, allEvents, FALSE, WSA_INFINITE, FALSE) };
        if (waitResult == WSA_WAIT_EVENT_0)
        {
            break;
        }
        ::ZeroMemory(&networkEvents, sizeof(networkEvents));
        SOCKET sock{ mSockets[waitResult - WSA_WAIT_EVENT_0 - 1] };
        int enumResult{ ::WSAEnumNetworkEvents(sock,
                allEvents[waitResult - WSA_WAIT_EVENT_0], &networkEvents) };
        assert(enumResult != SOCKET_ERROR);
        switch (waitResult)
        {
        case WSA_WAIT_EVENT_0 + 1:
        {
            if (networkEvents.lNetworkEvents & FD_ACCEPT)
            {
                // TODO: Fix this mess!
                if (eventNum == 2) // For now can only accept one client ever
                {
                    assert(networkEvents.iErrorCode[FD_ACCEPT_BIT] == 0);
                    mSockets[1] = acceptClient();
                    ++eventNum;
                    allEvents[2] = ::WSACreateEvent();
                    assert(WSA_INVALID_EVENT != allEvents[2]);
                    int selectResult{ ::WSAEventSelect(mSockets[1], allEvents[2], FD_READ | FD_WRITE | FD_CLOSE) };
                    assert(selectResult != SOCKET_ERROR);
                }
            }
            break;
        }
        case WSA_WAIT_EVENT_0 + 2:
            if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
            {
                ::OutputDebugString(L"Some action!\n");
                assert(!(networkEvents.lNetworkEvents & FD_READ) || networkEvents.iErrorCode[FD_READ_BIT] == 0);
                assert(!(networkEvents.lNetworkEvents & FD_WRITE) || networkEvents.iErrorCode[FD_WRITE_BIT] == 0);

                if (networkEvents.lNetworkEvents & FD_READ)
                {
                    DWORD received{ 0 };
                    DWORD flags{ 0 };
                    // TODO: read more
                    int recvResult{ ::WSARecv(sock, &bufStruct, 1, &received, &flags, nullptr, nullptr) };
                    assert(recvResult != SOCKET_ERROR);

                    DWORD processed{offset};
                    bool stop{false};
                    while (!stop && processed < received)
                    {
                        switch (mState)
                        {
                        case State::Command:
                        {
                            Command command = *reinterpret_cast<Command *>(buf + processed);
                            mExpectedValue = command;
                            mState = State::Value;
                            processed += sizeof(command);
                            break;
                        }
                        case State::Value:
                        {
                            switch (mExpectedValue)
                            {
                            case Command::Direction:
                            {
                                if (processed + sizeof(KeyType) > received)
                                {
                                    ::memcpy(buf, buf + processed, received - processed);
                                    stop = true;
                                }
                                else
                                {
                                    KeyType incoming =
                                        *reinterpret_cast<KeyType *>(buf + processed);
                                    ::OutputDebugString(L"One short!\n");
                                    mKeyCallback(incoming);
                                    processed += sizeof(KeyType);
                                }
                                break;
                            }
                            case Command::Step:
                                if (processed + sizeof(KeyType) > received)
                                {
                                    ::memcpy(buf, buf + processed, received - processed);
                                    stop = true;
                                }
                                else
                                {
                                    unsigned incoming =
                                        *reinterpret_cast<unsigned *>(buf + processed);
                                    ::OutputDebugString(L"One unsigned!\n");
                                    mStepCallback(incoming);
                                    processed += sizeof(incoming);
                                }
                                break;
                            }
                            mState = State::Command;
                            break;
                        }
                        }
                    }
                    offset = received - processed;
                    //for (DWORD processed{0}; processed < received; processed += sizeof(KeyType))
                    //{
                    //    KeyType incoming = *reinterpret_cast<KeyType *>(buf + processed);
                    //    ::OutputDebugString(L"One short!\n");
                    //    mReceiveCallback(incoming);
                    //}
                }
            }
            break;
        }
    }

    if(allEvents[2] != WSA_INVALID_EVENT)
        ::WSACloseEvent(allEvents[2]);

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