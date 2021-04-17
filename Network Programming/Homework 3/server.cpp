#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

typedef struct Session
{
    SOCKET connectedSocket;
    char *clientIP;
    int clientPort;
    char *username;
    bool isLoggedIn;
} Session;

/* echoThread - Thread to receive the message from client and echo*/
unsigned __stdcall echoThread(void *param)
{
    char buff[BUFF_SIZE];
    int ret;

    SOCKET connectedSocket = (SOCKET)param;
    while (1)
    {
        // Receive message from client
        ret = recv(connectedSocket, buff, BUFF_SIZE, 0);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: Cannot receive data.\n", WSAGetLastError());
            break;
        }
        else if (ret == 0)
        {
            printf("Client disconnects.\n");
            break;
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
                    break;
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
    }

    closesocket(connectedSocket);
    return 0;
}

/* handleRequestThread - Thread to receive the request from client and handle */
unsigned __stdcall handleRequestThread(void *param)
{
    char buff[BUFF_SIZE];
    int ret;

    // Get clientIP and clientPort from connectedSocket
    sockaddr_in clientAddr;
    char clientIP[INET_ADDRSTRLEN];
    int clientAddrLen = sizeof(clientAddr), clientPort;
    if (getpeername((SOCKET)param, (sockaddr *)&clientAddr, &clientAddrLen) == SOCKET_ERROR)
    {
        printf("Error %d: cannot get socket name.\n", WSAGetLastError());
    }
    else
    {
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
        clientPort = ntohs(clientAddr.sin_port);
    }

    // Create session
    char username[BUFF_SIZE] = "";
    Session sess = {(SOCKET)param, clientIP, clientPort, username, false};

    while (1)
    {
        // Receive message from client
        ret = recv(sess.connectedSocket, buff, BUFF_SIZE, 0);
        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAECONNRESET)
            {
                printf("Connection [%s:%d] reset by peer.\n", sess.clientIP, sess.clientPort);
                // TODO update session
            }
            else
            {
                printf("Error %d: Cannot receive data.\n", WSAGetLastError());
            }
            break;
        }
        else if (ret == 0)
        {
            printf("Unexpected error.\n");
            break;
        }
        else
        {
            buff[ret] = 0;
            printf("Receive from client [%s:%d]: %s\n", sess.clientIP, sess.clientPort, buff);

            // Resolve request
            char mode[5];
            memcpy_s(mode, 5, &buff[0], 4);
            mode[4] = 0;
            if (strcmp(mode, "USER") == 0)
            /**
             * USER request return code:
             * 10:  success
             * 111: logged in already
             * 112: username doesn't exist
             * 113: account is being locked
             */
            {
                if (sess.isLoggedIn)
                {
                    strcpy_s(buff, BUFF_SIZE, "111");
                }
                else
                {
                    char inputUsername[BUFF_SIZE] = "";
                    memcpy_s(inputUsername, BUFF_SIZE, &buff[5], strlen(buff) - 5);

                    FILE *accPtr = fopen("account.txt", "r");
                    if (accPtr == NULL)
                    {
                        printf("Cannot open database file. Error code: %d.\n", errno);
                        break;
                    }
                    else
                    {
                        char accTrack[BUFF_SIZE] = "";
                        bool accExist = false;
                        while (fgets(accTrack, BUFF_SIZE, accPtr) != NULL)
                        {
                            int spacePos = 0;
                            while (spacePos < strlen(accTrack))
                            {
                                if (accTrack[spacePos] != ' ')
                                {
                                    ++spacePos;
                                }
                                else
                                {
                                    break;
                                }
                            }
                            char active, nameTmp[BUFF_SIZE] = "";
                            memcpy_s(nameTmp, BUFF_SIZE, accTrack, spacePos);
                            nameTmp[spacePos] = 0;
                            active = accTrack[spacePos + 1];
                            if (strcmp(nameTmp, inputUsername) == 0)
                            {
                                accExist = true;
                                if (active == '1')
                                {
                                    strcpy_s(buff, BUFF_SIZE, "113");
                                }
                                else if (active == '0')
                                {
                                    strcpy_s(buff, BUFF_SIZE, "10");
                                    sess.isLoggedIn = true;
                                    strcpy_s(sess.username, BUFF_SIZE, inputUsername);
                                }
                                break;
                            }
                        }
                        if (!sess.isLoggedIn && !accExist)
                        {
                            strcpy_s(buff, BUFF_SIZE, "112");
                        }
                    }
                    fclose(accPtr);
                }
            }
            else if (strcmp(mode, "POST") == 0)
            {
                printf("POST!\n");
            }
            else if (strcmp(mode, "QUIT") == 0)
            {
                printf("QUIT!\n");
            }
            else
            {
                printf("Unexpected mode.\n");
                break;
            }

            // Echo to client
            ret = send(sess.connectedSocket, buff, strlen(buff), 0);
            if (ret == SOCKET_ERROR)
            {
                printf("Error %d: Cannot send data.\n", WSAGetLastError());
            }
        }
    }
    closesocket(sess.connectedSocket);
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
    int clientAddrLen = sizeof(clientAddr);
    while (1)
    {
        connSock = accept(listenSock, (sockaddr *)&clientAddr, &clientAddrLen);
        if (connSock == SOCKET_ERROR)
        {
            printf("Error %d: cannot permit incoming connection.\n", WSAGetLastError());
        }
        else
        {
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            int clientPort = ntohs(clientAddr.sin_port);
            printf("Accept incoming connection from %s:%d\n", clientIP, clientPort);

            _beginthreadex(0, 0, handleRequestThread, (void *)connSock, 0, 0); // start thread
        }
    }

    // Step 5: Close socket
    closesocket(listenSock);

    // Step 6: Terminate WinSock
    WSACleanup();

    return 0;
}
