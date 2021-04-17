#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

void sendLoginRequest(SOCKET client)
{
    // Create message
    char buff[BUFF_SIZE] = "", username[BUFF_SIZE] = "";
    strcat_s(buff, BUFF_SIZE, "USER ");
    printf("Enter your username: ");
    scanf("%s", username);
    fflush(stdin);
    strcat_s(buff, BUFF_SIZE, username);

    // Send message
    int ret = send(client, buff, strlen(buff), 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Error %d: cannot send request.\n", WSAGetLastError());
    }
}

void sendPostRequest(SOCKET client)
{
    // Create message
    char buff[BUFF_SIZE] = "", message[BUFF_SIZE] = "";
    strcat_s(buff, BUFF_SIZE, "POST ");
    printf("Enter message to post: ");
    fgets(message, 1024, stdin);
    message[strlen(message) - 1] = '\0';
    fflush(stdin);
    strcat_s(buff, BUFF_SIZE, message);

    // Send message
    int ret = send(client, buff, strlen(buff), 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Error %d: cannot send request.\n", WSAGetLastError());
    }
}

void sendLogoutRequest(SOCKET client)
{
    // Create message
    char buff[BUFF_SIZE] = "QUIT";

    // Send message
    int ret = send(client, buff, strlen(buff), 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Error %d: cannot send request.\n", WSAGetLastError());
    }
}

void resolveServerResponse(SOCKET client) {}

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
    SOCKET client;
    client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET)
    {
        printf("Error %d: cannot create server socket.\n", WSAGetLastError());
        return 0;
    }
    printf("Client started!\n");

    // (optional) Set time-out for receiving
    int tv = 10000; // Time-out interval: 10000ms
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
        printf("Menu:\n1.\tLog in\n2.\tPost \n3.\tLog out\n");
        int mode = 0;
        while (mode < 1 || mode > 3)
        {
            printf("Choose mode: ");
            scanf("%d", &mode);
            fflush(stdin);
            if (mode < 1 || mode > 3)
            {
                printf("Error: your mode is %d, not in range [1, 3].\n", mode);
                fflush(stdin);
            }
        }
        switch (mode)
        {
        case 1:
            sendLoginRequest(client);
            break;
        case 2:
            sendPostRequest(client);
            break;
        case 3:
            sendLogoutRequest(client);
            break;
        default:
            printf("unexpected error?!");
            break;
        }

        /*
        // Send message
        printf("Send to server: ");
        fgets(buff, BUFF_SIZE, stdin);
        buff[strlen(buff) - 1] = 0; // fgets() identifies '\n' is the last character

        ret = send(client, buff, strlen(buff), 0);
        if (ret != SOCKET_ERROR && strlen(buff) == 0)
        {
            break;
        }
        */

        // Receive message
        ret = recv(client, buff, BUFF_SIZE, 0);

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
        else
        {
            buff[ret] = 0;
            printf("Receive from server: %s\n", buff);
            resolveServerResponse(client);
        }
    }

    // Step 6: close socket
    closesocket(client);

    // Step 7: terminate WinSock
    WSACleanup();

    return 0;
}
