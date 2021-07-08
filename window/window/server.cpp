#include "server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networkLib/winSock.h>
#include <networkLib/socket.h>
#include <networkLib/addrInfo.h>
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

namespace Server
{

void serve()
{
    int iResult;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    Network::WinSock winSock;
    Network::Socket clientSocket;

    {
        Network::Socket listenSocket;
        {
            struct addrinfo hints;
            ::ZeroMemory(&hints, sizeof(hints));
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            hints.ai_flags = AI_PASSIVE;

            Network::AddrInfo addrInfo{ DEFAULT_PORT, &hints };
            // Create a SOCKET for connecting to server
            listenSocket = Network::Socket{ addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol };
            if (listenSocket == INVALID_SOCKET) {
                return;
            }

            // Setup the TCP listening socket
            iResult = ::bind(listenSocket, addrInfo->ai_addr, (int)addrInfo->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                return;
            }
        }

        iResult = ::listen(listenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            return;
        }

        // Accept a client socket
        clientSocket = ::accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            return;
        }
    }

    // Receive until the peer shuts down the connection
    do {

        iResult = ::recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            //printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = ::send(clientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                return;
            }
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
    if (iResult == SOCKET_ERROR) {
        return;
    }
}

}