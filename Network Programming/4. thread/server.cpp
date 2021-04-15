#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

/* echoThread - Thread to receive the message from client and echo*/
unsigned __stdcall echoThread(void *param)
{
    char buff[BUFF_SIZE];
    int ret;

    SOCKET connectedSocket = (SOCKET)param;
    ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Error %d: Cannot receive data.\n", WSAGetLastError());
    }
    else if (ret == 0)
    {
        printf("Client disconnects.\n");
    }
    else if (strlen(buff) > 0)
    {
        buff[ret] = 0;

        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        if (getpeername(connectedSocket, (sockaddr *)&clientAddr, &clientAddrLen) == SOCKET_ERROR)
        {
            printf("Error %d: cannot get socket name.\n", WSAGetLastError());
        }
        else
        {
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            int clientPort = ntohs(clientAddr.sin_port);
            printf("Receive from client [%s:%d]: %s\n", clientIP, clientPort, buff);

            if (_stricmp(buff, "") == 0)
            {
                printf("Disconnect to %s:%d.\n", clientIP, clientPort);
            }
            else
            {
                //Echo to client
                ret = send(connectedSocket, buff, strlen(buff), 0);
                if (ret == SOCKET_ERROR)
                {
                    printf("Error %d: Cannot send data.\n", WSAGetLastError());
                }
            }
        }
    }

    closesocket(connectedSocket);
    return 0;
}

int main(int argc, char *argv[])
{
    // Step 1: Initiate WinSock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    if (WSAStartup(wVersion, &wsaData))
    {
        printf("Error %d: WinSock 2.2 is not supported.\n", WSAGetLastError());
        return 0;
    }

    // Step 2: Construct socket
    SOCKET listenSock;
    listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET)
    {
        printf("Error %d: Cannot create server socket.", WSAGetLastError());
        return 0;
    }

    // Step 3: Bind address to socket
    if (argc != 2)
    {
        printf("Arguments are invalid.\n");
        return 0;
    }
    int svPort = atoi(argv[1]);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(svPort);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
    if (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error %d: cannot associate a local address with this address.\n", WSAGetLastError());
        return 0;
    }

    // Step 4: Listen request from client
    if (listen(listenSock, 10))
    {
        printf("Error %d: cannot place server socket in state LISTEN.\n", WSAGetLastError());
        return 0;
    }
    printf("Server started!\n");

    // Step 5: communicate with client
    SOCKET connSock;
    sockaddr_in clientAddr;
    char clientIP[INET_ADDRSTRLEN];
    int clientAddrLen = sizeof(clientAddr), clientPort;
    while (1)
    {
        connSock = accept(listenSock, (sockaddr *)&clientAddr, &clientAddrLen);
        if (connSock == SOCKET_ERROR)
        {
            printf("Error %d: cannot permit incoming connection.\n", WSAGetLastError());
        }
        else
        {
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            clientPort = ntohs(clientAddr.sin_port);
            printf("Accept incoming connection from %s:%d\n", clientIP, clientPort);
            _beginthreadex(0, 0, echoThread, (void *)connSock, 0, 0); //start thread
        }
    }

    // Step 5: Close socket
    closesocket(listenSock);

    // Step 6: Terminate WinSock
    WSACleanup();

    return 0;
}
