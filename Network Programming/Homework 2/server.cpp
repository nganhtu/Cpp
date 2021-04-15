#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

char *splitLettersAndDigits(char *buff, int length)
{
    char *res = (char *)calloc(BUFF_SIZE, sizeof(char));
    char letters[BUFF_SIZE] = "", digits[BUFF_SIZE] = "", currChar[2];
    for (int i = 0; i < length; ++i)
    {
        if (buff[i] >= '0' && buff[i] <= '9')
        {
            memcpy(currChar, &buff[i], 1); // get substring contains only 1 char
            currChar[1] = 0;
            strcat_s(digits, BUFF_SIZE, (const char *)currChar);
        }
        else if ((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z'))
        {
            memcpy(currChar, &buff[i], 1); // get substring contains only 1 char
            currChar[1] = 0;
            strcat_s(letters, BUFF_SIZE, (const char *)currChar);
        }
        else
        {
            strcpy_s(res, BUFF_SIZE, "-");
            return res;
        }
    }
    strcpy_s(res, BUFF_SIZE, "+");
    strcat_s(res, BUFF_SIZE, letters);
    strcat_s(res, BUFF_SIZE, "\n");
    strcat_s(res, BUFF_SIZE, digits);

    return res;
}

int main(int argc, char *argv[])
{
    // Step 1: Initiate WinSock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    while (WSAStartup(wVersion, &wsaData))
    {
        printf("Error %d: WinSock 2.2 is not supported.\n", WSAGetLastError());
    }

    // Step 2: Construct socket
    SOCKET listenSock;
    do
    {
        listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listenSock == INVALID_SOCKET)
        {
            printf("Error %d: cannot create server socket.\n", WSAGetLastError());
        }
    } while (listenSock == INVALID_SOCKET);

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
    while (bind(listenSock, (sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error %d: cannot associate a local address with this address.\n", WSAGetLastError());
    }
    printf("Server started!\n");

    // Step 4: Listen request from client
    while (listen(listenSock, 10))
    {
        printf("Error %d: cannot place server socket in state LISTEN.\n", WSAGetLastError());
    }

    // Step 5: communicate with client
    sockaddr_in clientAddr;
    char buff[BUFF_SIZE], clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr), clientPort;
    SOCKET connSock;
    while (1)
    {
        // accept request
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
                printf("Accept incoming connection from %s:%d.\n", clientIP, clientPort);
                break;
            }
        }

        while (1)
        {
            // Receive message from client
            ret = recv(connSock, buff, BUFF_SIZE, 0);
            if (ret == SOCKET_ERROR)
            {
                printf("Error %d: cannot receive data.\n", WSAGetLastError());
                break;
            }
            else
            {

                buff[ret] = 0;
                printf("Receive from client [%s:%d]: %s\n", clientIP, clientPort, buff);

                if (_stricmp(buff, "") == 0)
                {
                    printf("Disconnect to %s:%d.\n", clientIP, clientPort);
                    closesocket(connSock);
                    break;
                }

                char *res = splitLettersAndDigits(buff, ret);

                ret = send(connSock, res, strlen(res), 0);
                if (ret == SOCKET_ERROR)
                {
                    printf("Error %d: cannot send data.\n", WSAGetLastError());
                    break;
                }
            }
        } // End communicating

        // Step 5: Close socket
        closesocket(connSock);
    }
    closesocket(listenSock);

    // Step 6: Terminate WinSock
    WSACleanup();

    return 0;
}
