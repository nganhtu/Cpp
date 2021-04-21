#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define BUFF_SIZE 2048
#define MAX_MESS_SIZE 204800
#define DELIMITER "\r\n"
#pragma comment(lib, "Ws2_32.lib")

void sendRequestWithDelimiter(SOCKET client, const char *originalMessage)
{
    // Insert delimiter
    char message[MAX_MESS_SIZE];
    strcpy_s(message, MAX_MESS_SIZE, originalMessage);
    strcat_s(message, MAX_MESS_SIZE, DELIMITER);

    // Split then send message
    char buff[BUFF_SIZE];
    strcpy_s(buff, BUFF_SIZE, "");
    int cnt = (int)strlen(message) / (BUFF_SIZE - 2), currSplit = 0;
    while (cnt--)
    {
        char buff[BUFF_SIZE];
        memcpy_s(buff, BUFF_SIZE, &message[currSplit], BUFF_SIZE - 2);
        buff[BUFF_SIZE - 2] = '\0';
        int ret = send(client, buff, strlen(buff), 0);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: cannot send request.\n", WSAGetLastError());
        }
        currSplit += BUFF_SIZE - 2;
    }
    if (message[currSplit] != '\0')
    {
        char buff[BUFF_SIZE];
        memcpy_s(buff, BUFF_SIZE, &message[currSplit], strlen(message) - currSplit);
        buff[strlen(message) - currSplit] = '\0';
        int ret = send(client, buff, strlen(buff), 0);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: cannot send request.\n", WSAGetLastError());
        }
    }
}

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
    sendRequestWithDelimiter(client, buff);
}

void sendPostRequest(SOCKET client)
{
    // Create message
    char buff[MAX_MESS_SIZE] = "", message[MAX_MESS_SIZE] = "";
    strcat_s(buff, BUFF_SIZE, "POST ");
    printf("Enter message to post: ");
    fgets(message, MAX_MESS_SIZE, stdin);
    fflush(stdin);
    message[strlen(message) - 1] = '\0';
    strcat_s(buff, MAX_MESS_SIZE, message);

    // Send message
    sendRequestWithDelimiter(client, buff);
}

void sendLogoutRequest(SOCKET client)
{
    // Create message
    char buff[BUFF_SIZE] = "QUIT";

    // Send message
    sendRequestWithDelimiter(client, buff);
}

void handleServerResponse(SOCKET client, char *serverResponse)
{
    if (strcmp(serverResponse, "100") == 0)
    {
        printf("Log in successfully.\n");
    }
    else if (strcmp(serverResponse, "111") == 0)
    {
        printf("Error: you are logged in already.\n");
    }
    else if (strcmp(serverResponse, "112") == 0)
    {
        printf("Error: username doesn't exist.\n");
    }
    else if (strcmp(serverResponse, "113") == 0)
    {
        printf("Error: account is being locked.\n");
    }
    else if (strcmp(serverResponse, "200") == 0)
    {
        printf("Post message successfully.\n");
    }
    else if (strcmp(serverResponse, "211") == 0)
    {
        printf("Error: you are currently not logged in.\n");
    }
    else if (strcmp(serverResponse, "300") == 0)
    {
        printf("Logout succesfully.\n");
    }
    else if (strcmp(serverResponse, "311") == 0)
    {
        printf("Error: you are currently not logged in.\n");
    }
    else
    {
        printf("Unexpected return code from server: %s\n", serverResponse);
        closesocket(client);
        WSACleanup();
        exit(0);
    }
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
            handleServerResponse(client, buff);
        }
    }

    // Step 6: close socket
    closesocket(client);

    // Step 7: terminate WinSock
    WSACleanup();

    return 0;
}
