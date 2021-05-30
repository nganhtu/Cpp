#include <stdio.h>
#pragma warning(disable : 4996)
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <time.h>
#define SERVER_ADDR "127.0.0.1"
#define BUFF_SIZE 2048
#define RESPCODELEN 3
#define DELIMITER "\r\n"
#pragma comment(lib, "Ws2_32.lib")

typedef struct Status
{
    char *clientIP;
    int clientPort;
    char *username;
    bool isLoggedIn;
} Status;

void setStatus(Status *status, const char *clientIP, int clientPort)
{
    char _clientIP[BUFF_SIZE];
    strcpy_s(_clientIP, BUFF_SIZE, clientIP);
    status->clientIP = _clientIP;

    status->clientPort = clientPort;

    char _username[BUFF_SIZE];
    strcpy_s(_username, BUFF_SIZE, "");
    status->username = _username;

    status->isLoggedIn = false;
}

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
        char timeStr[100];
        currTime = time(0);
        strftime(timeStr, 100, "%d/%m/%Y %H:%M:%S", localtime(&currTime));

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

/** Handle client request and generate response code.
 * @param response pointer to output string that contains response code
 * @param request pointer to input string that contains client request
 * @param status pointer to the status of current client
 * @return 1 if handle successfully; -1 if an error occured.
 */
int handleRequest(char *response, const char *request, Status *status)
{
    char res[BUFF_SIZE];

    // Resolve request
    if (strlen(request) < 4)
    {
        // return code 000: unexpected request
        strcpy_s(res, BUFF_SIZE, "000");
    }
    else
    {
        char mode[BUFF_SIZE];
        memcpy_s(mode, BUFF_SIZE, &request[0], 4);
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
            if (status->isLoggedIn)
            {
                strcpy_s(res, BUFF_SIZE, "111");
            }
            else
            {
                char inputUsername[BUFF_SIZE] = "";
                memcpy_s(inputUsername, BUFF_SIZE, &request[5], strlen(request) - 5);

                FILE *accPtr = fopen("account.txt", "r");
                if (accPtr == NULL)
                {
                    printf("Cannot open database file. Error code: %d.\n", errno);
                    return -1;
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
                                strcpy_s(res, BUFF_SIZE, "113");
                            }
                            else if (active == '0')
                            {
                                strcpy_s(res, BUFF_SIZE, "100");
                                status->isLoggedIn = true;
                                strcpy_s(status->username, BUFF_SIZE, inputUsername);
                            }
                            break;
                        }
                    }
                    if (!status->isLoggedIn && !accExist)
                    {
                        strcpy_s(res, BUFF_SIZE, "112");
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
            if (!status->isLoggedIn)
            {
                strcpy_s(res, BUFF_SIZE, "211");
            }
            else
            {
                char inputMessage[BUFF_SIZE] = "";
                memcpy_s(inputMessage, BUFF_SIZE, &request[5], strlen(request) - 5);
                strcpy_s(res, BUFF_SIZE, "200");
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
            if (!status->isLoggedIn)
            {
                strcpy_s(res, BUFF_SIZE, "311");
            }
            else
            {
                strcpy_s(res, BUFF_SIZE, "300");
                strcpy_s(status->username, BUFF_SIZE, "");
                status->isLoggedIn = false;
            }
        }
        else
        {
            // return code 000: unexpected request
            strcpy_s(res, BUFF_SIZE, "000");
        }
    }

    memcpy_s(response, BUFF_SIZE, res, RESPCODELEN);
    response[RESPCODELEN] = 0;

    recordToLog(status->clientIP, status->clientPort, request, response);

    return 1;
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
        printf("Error %d: Cannot create server socket.\n", WSAGetLastError());
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

    SOCKET client[FD_SETSIZE], connSock;
    fd_set readfds, initfds; // use initfds to initiate readfds at the begining of every loop step
    sockaddr_in clientAddr;
    int ret, nEvents, clientAddrLen;
    char recvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];

    for (int i = 0; i < FD_SETSIZE; ++i)
    {
        client[i] = 0; // 0 indicates available entry
    }
    FD_ZERO(&initfds);
    FD_SET(listenSock, &initfds);

    Status status[FD_SETSIZE];

    // Step 5: Communicate with clients
    while (1)
    {
        readfds = initfds; /* structure assignment */
        nEvents = select(0, &readfds, 0, 0, 0);
        if (nEvents < 0)
        {
            printf("Error %d: cannot poll sockets.\n", WSAGetLastError());
            break;
        }

        // new client connection
        if (FD_ISSET(listenSock, &readfds))
        {
            clientAddrLen = sizeof(clientAddr);
            if ((connSock = accept(listenSock, (sockaddr *)&clientAddr, &clientAddrLen)) < 0)
            {
                printf("Error %d: cannot accept new connection.\n", WSAGetLastError());
                break;
            }
            else
            {
                char clientIP[INET_ADDRSTRLEN];
                int clientAddrLen = sizeof(clientAddr), clientPort;
                if (getpeername(connSock, (sockaddr *)&clientAddr, &clientAddrLen) == SOCKET_ERROR)
                {
                    printf("Error %d: cannot get socket name.\n", WSAGetLastError());
                }
                else
                {
                    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
                    clientPort = ntohs(clientAddr.sin_port);
                    printf("Accept connection from [%s:%d].\n", clientIP, clientPort);
                }

                int i;
                for (i = 0; i < FD_SETSIZE; ++i)
                {
                    if (client[i] == 0)
                    {
                        client[i] = connSock;
                        FD_SET(client[i], &initfds);
                        setStatus(&status[i], clientIP, clientPort);
                        break;
                    }
                }

                if (i == FD_SETSIZE)
                {
                    printf("Too many clients.\n");
                    closesocket(connSock);
                }

                if (--nEvents == 0)
                {
                    continue; // no more event
                }
            }
        }

        // Communicate with clients
        for (int i = 0; i < FD_SETSIZE; ++i)
        {
            if (client[i] == 0)
            {
                continue;
            }

            if (FD_ISSET(client[i], &readfds))
            {
                // Receive message from client
                ret = recv(client[i], recvBuff, BUFF_SIZE, 0);
                if (ret <= 0)
                {
                    if (WSAGetLastError() == WSAECONNRESET)
                    {
                        printf("Connection [%s:%d] reset by peer.\n", status[i].clientIP, status[i].clientPort);
                    }
                    else
                    {
                        printf("Error %d: cannot receive data.\n", WSAGetLastError());
                    }
                    FD_CLR(client[i], &initfds);
                    closesocket(client[i]);
                    client[i] = 0;
                }
                else
                {
                    recvBuff[ret] = 0;
                    printf("Receive from client [%s:%d]: \"%s\"\n", status[i].clientIP, status[i].clientPort, recvBuff);

                    // Handle request and response to client
                    handleRequest(sendBuff, recvBuff, &status[i]);
                    ret = send(client[i], sendBuff, RESPCODELEN, 0);
                    if (ret == SOCKET_ERROR)
                    {
                        printf("Error %d: cannot send data.\n", WSAGetLastError());
                    }
                }
            }

            if (--nEvents <= 0)
            {
                continue; // no more event
            }
        }
    }

    // Step 6: Close socket
    closesocket(listenSock);

    // Step 7: Terminate WinSock
    WSACleanup();

    return 0;
}
