#include "clientConnection.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <networkLib/winSock.h>
#include <networkLib/socket.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

void sendToServer()
{
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    Network::WinSock winSock;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = ::getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        return;
        //return 1;
    }

    Network::Socket connectSocket;

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        connectSocket = Network::Socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (!connectSocket.isValid()) {
            return;
        }

        // Connect to server.
        iResult = ::connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            connectSocket = Network::Socket();
            continue;
        }
        break;
    }

    ::freeaddrinfo(result);

    if (!connectSocket.isValid()) {
        return;
    }

    // Send an initial buffer
    iResult = ::send(connectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        return;
    }

    //printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = ::shutdown(connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        return;
    }

    // Receive until the peer closes the connection
    do {

        iResult = ::recv(connectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            ;// printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            ;// printf("Connection closed\n");
        else
            ;// printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
}
