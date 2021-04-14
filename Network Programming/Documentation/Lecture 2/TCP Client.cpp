// Sửa chương trình client: kết thúc gửi dữ liệu sau xâu "BYE", "bye", "Bye",...

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define SERVER_PORT 5500
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

int main()
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
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

    // Step 4: Request to connect server
    if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error %d: cannot connect server.\n", WSAGetLastError());
        return 0;
    }

    printf("Connected server!\n");

    // Step 5: Communicate with server
    char buff[BUFF_SIZE];
    int ret;
    while (1)
    {
        // Send message
        printf("Send to server: ");
        fgets(buff, BUFF_SIZE, stdin);
        buff[strlen(buff) - 1] = '\0'; // fgets() identifies '\n' is the last character

        ret = send(client, buff, strlen(buff), 0);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: cannot send data.\n", WSAGetLastError());
        }
        if (strlen(buff) == 0)
        {
            break;
        }

        // Receive message
        ret = recv(client, buff, strlen(buff), 0);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
                printf("Time-out!\n");
            }
            else
            {
                printf("Error %d: cannot receive data.\n", WSAGetLastError());
            }
        }
        else if (strlen(buff) > 0)
        {
            buff[ret] = 0;
            printf("Receive from server: %s\n", buff);
        }
    }

    // Step 6: close socket
    closesocket(client);

    // Step 7: terminate WinSock
    WSACleanup();

    return 0;
}
