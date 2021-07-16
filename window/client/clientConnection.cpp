#include "clientConnection.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networkLib/addrInfo.h>
#include <assert.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void ClientConnection::sendToServer()
{
    const char* sendbuf = "this is a test";
    int iResult{ ::send(mConnectSocket, sendbuf, (int)strlen(sendbuf), 0) };
    assert(iResult != SOCKET_ERROR);
}

void ClientConnection::shutdown()
{
    int iResult{ ::shutdown(mConnectSocket, SD_SEND) };
    assert(iResult != SOCKET_ERROR);
}

void ClientConnection::init()
{
    //char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    //int recvbuflen = DEFAULT_BUFLEN;

    {
        addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        Network::AddrInfo addrInfo{ "localhost", DEFAULT_PORT, &hints };

        // Attempt to connect to an address until one succeeds
        for (addrinfo* ptr{ addrInfo }; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            mConnectSocket = Network::Socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (!mConnectSocket.isValid()) {
                return;
            }

            // Connect to server.
            iResult = ::connect(mConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                mConnectSocket = Network::Socket();
                continue;
            }
            break;
        }
    }

    assert(mConnectSocket.isValid());

    // Send an initial buffer


    //printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent


    //// Receive until the peer closes the connection
    //do {

    //    iResult = ::recv(connectSocket, recvbuf, recvbuflen, 0);
    //    if (iResult > 0)
    //        ;// printf("Bytes received: %d\n", iResult);
    //    else if (iResult == 0)
    //        ;// printf("Connection closed\n");
    //    else
    //    {
    //        
    //        int err{ WSAGetLastError() };
    //        ::OutputDebugString(L"ERROR");
    //        ;// printf("recv failed with error: %d\n", WSAGetLastError());
    //    }

    //} while (iResult > 0);
}
