#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

char *interpret(char *buff, int length)
{
    char *res = (char *)calloc(BUFF_SIZE, sizeof(char));
    if (buff[0] == '+')
    {
        char subBuff[BUFF_SIZE];
        memcpy_s(subBuff, BUFF_SIZE, &buff[1], (length - 1) * sizeof(char));
        subBuff[length - 1] = 0;

        strcpy_s(res, BUFF_SIZE, "Receive from server: ");
        strcat_s(res, BUFF_SIZE, subBuff);
        strcat_s(res, BUFF_SIZE, "\n");
        return res;
    }
    else if (buff[0] == '-')
    {
        strcpy_s(res, BUFF_SIZE, "Error: string not contains only letters and digits.\n");
        return res;
    }

    return NULL;
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
        // Send message
        while (1)
        {
            printf("Send to server: ");
            fgets(buff, BUFF_SIZE, stdin);
            buff[strlen(buff) - 1] = 0; // fgets() identifies '\n' is the last character

            if (strlen(buff) > BUFF_SIZE - 2) // result from server may contains '+' and '\n'
            {
                printf("Data is larger than buffer size.\n");
            }
            else
            {
                break;
            }
        }

        ret = send(client, buff, strlen(buff), 0);
        if (ret != SOCKET_ERROR && strlen(buff) == 0)
        {
            break;
        }

        // Receive message
        ret = recv(client, buff, BUFF_SIZE, 0);

        if (ret == SOCKET_ERROR && WSAGetLastError() == WSAETIMEDOUT)
        {
            /**
             * Reset connection:
             * After time-out, data still exists in server's connection queue (?)
             * therefore send() and recv() at client and server are not alternate.
             * So I reset connection to clear data at queue,
             * but connection order will be changed.
             */
            printf("Time-out! Connection will be reseted.\n");
            closesocket(client);
            client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (client == INVALID_SOCKET)
            {
                return 0;
            }
            int tv = 10000;
            setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&tv), sizeof(int));
            if (connect(client, (sockaddr *)&serverAddr, sizeof(serverAddr)))
            {
                return 0;
            }
            printf("Connected server!\n");
        }
        else if (ret != SOCKET_ERROR && strlen(buff) > 0)
        {
            buff[ret] = 0;
            char *message = interpret(buff, ret);
            printf("%s", message);
        }
    }

    // Step 6: close socket
    closesocket(client);

    // Step 7: terminate WinSock
    WSACleanup();

    return 0;
}
