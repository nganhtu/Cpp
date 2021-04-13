#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
    // Step 1: Inittiate WinSock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    if (WSAStartup(wVersion, &wsaData))
    {
        printf("Error %d: WinSock 2.2 is not supported.\n", WSAGetLastError());
        return 0;
    }

    // Step 2: Construct socket
    SOCKET client;
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client == INVALID_SOCKET)
    {
        printf("Error %d: cannot create server socket.\n", WSAGetLastError());
        return 0;
    }
    printf("Client started!\n");
    // (optional) Set time-out for receiving
    int tv = 10000; //Time-out interval: 10000ms
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&tv), sizeof(int));

    // Step 3: Specify server address
    if (argc != 3)
    {
        printf("Arguments are invalid.\n");
        return 0;
    }
    char svAddr[13] = "";
    strcpy_s(svAddr, argv[1]);
    int svPort = atoi(argv[2]);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(svPort);
    inet_pton(AF_INET, svAddr, &serverAddr.sin_addr);

    // Step 4: Communicate with server
    char buff[BUFF_SIZE];
    int ret, serverAddrLen = sizeof(serverAddr);

    while (1)
    {
        // Send message
        printf("Send to server: ");
        fgets(buff, BUFF_SIZE, stdin);
        if (strlen(buff) == 0)
        {
            break;
        }

        ret = sendto(client, buff, strlen(buff), 0, (sockaddr *)&serverAddr, serverAddrLen);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: cannot send message.\n", WSAGetLastError());
        }

        // Receive message
        ret = recvfrom(client, buff, BUFF_SIZE, 0, (sockaddr *)&serverAddr, &serverAddrLen);

        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
                printf("Time-out!\n");
            }
            else
            {
                printf("Error %d: cannot receive message.\n", WSAGetLastError());
            }
        }
        else if (strlen(buff) > 0)
        {
            if (ret < BUFF_SIZE)
            {
                buff[ret] = 0;
                printf("Receive from server:\n%s\n", buff);
            }
        }
    } // end while

    // Step 5: Close socket
    closesocket(client);

    // Step 6: Terminate Winsock
    WSACleanup();

    return 0;
}
