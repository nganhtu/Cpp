#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <time.h>
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

void recordToLog(const char *clientIP, int clientPort, const char *message, const char *returnCode)
{
    FILE *logPtr = fopen("log_20184000.txt", "a");
    if (logPtr == NULL)
    {
        printf("Cannot open log file. Error code: %d\n", errno);
    }
    else
    {
        time_t currTime;
        time(&currTime);
        char timeStr[100];
        strcpy_s(timeStr, 100, ctime(&currTime));
        timeStr[strlen(timeStr) - 1] = '\0';

        fputs(clientIP, logPtr);
        fputs(":", logPtr);
        char buffTmp[BUFF_SIZE];
        fputs(itoa(clientPort, buffTmp, 10), logPtr);
        fputs(" [", logPtr);
        fputs((const char *)timeStr, logPtr);
        fputs("] $ ", logPtr);
        fputs(message, logPtr);
        fputs(" $ ", logPtr);
        fputs(returnCode, logPtr);
        fputs("\n", logPtr);
    }
    fclose(logPtr);
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
                strcpy_s(sess.username, BUFF_SIZE, "");
                sess.isLoggedIn = false;
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
            char clientRequest[BUFF_SIZE];
            strcpy_s(clientRequest, BUFF_SIZE, buff);

            // Resolve request
            char mode[5];
            memcpy_s(mode, 5, &buff[0], 4);
            mode[4] = 0;
            if (strcmp(mode, "USER") == 0)
            /**
             * USER request return code:
             * 
             * 100: success
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
                                    strcpy_s(buff, BUFF_SIZE, "100");
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
            /**
             * POST request return code:
             * 
             * 200: success
             * 211: not logged in
             */
            {
                if (!sess.isLoggedIn)
                {
                    strcpy_s(buff, BUFF_SIZE, "211");
                }
                else
                {
                    char inputMessage[BUFF_SIZE] = "";
                    memcpy_s(inputMessage, BUFF_SIZE, &buff[5], strlen(buff) - 5);
                    strcpy_s(buff, BUFF_SIZE, "200");
                }
            }
            else if (strcmp(mode, "QUIT") == 0)
            /**
             * QUIT request return code:
             * 
             * 300: success
             * 311: not logged in
             */
            {
                if (!sess.isLoggedIn)
                {
                    strcpy_s(buff, BUFF_SIZE, "311");
                }
                else
                {
                    strcpy_s(buff, BUFF_SIZE, "300");
                    strcpy_s(sess.username, BUFF_SIZE, "");
                    sess.isLoggedIn = false;
                }
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
            recordToLog(sess.clientIP, sess.clientPort, clientRequest, buff);
        }
    }
    printf("Disconnect to client [%s:%d].\n", sess.clientIP, sess.clientPort);
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
