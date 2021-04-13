#define _WINSOCK_DEPRECATED_NO_WARNINGS // for gethostbyaddr
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <iostream>
#include <string>
#include "winsock2.h"
#include "ws2tcpip.h"
#define SERVER_ADDR "127.0.0.1"
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
    SOCKET server;
    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server == INVALID_SOCKET)
    {
        printf("Error %d: cannot create server socket.\n", WSAGetLastError());
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
    if (bind(server, (sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        printf("Error %d: cannot bind this address.\n", WSAGetLastError());
        return 0;
    }
    printf("Server started!\n");

    // Step 4: Communicate with client
    sockaddr_in clientAddr;
    char buff[BUFF_SIZE], clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr), clientPort;

    while (1)
    {
        // Receive message
        ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
        if (ret == SOCKET_ERROR)
        {
            printf("Error %d: cannot receive data.\n", WSAGetLastError());
        }
        else if (strlen(buff) > 0)
        {
            buff[ret] = 0;
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            clientPort = ntohs(clientAddr.sin_port);
            printf("Receive from client [%s:%d]: %s\n", clientIP, clientPort, buff);

            // Prepare respond to client
            std::string str = "";

            // Identify type of request
            int countDot = 0;
            bool requestIsIPAddr = true;
            for (unsigned int i = 0; (i < strlen(buff)) && requestIsIPAddr; ++i)
            {
                if (buff[i] == '.')
                {
                    countDot++;
                }
                else if (buff[i] > '9' || buff[i] < '0')
                {
                    requestIsIPAddr = false;
                }
            }
            if (countDot != 3)
            {
                requestIsIPAddr = false;
            }

            if (requestIsIPAddr)
            // Get host informations corresponding to address
            {
                printf("Request is IP address.\n");

                char *addrStr;
                inet_pton(AF_INET, buff, &addrStr);
                hostent *host = gethostbyaddr((char *)&addrStr, sizeof(addrStr), AF_INET);
                if (host != nullptr)
                {
                    str += "+Official name: ";
                    str += host->h_name;
                    str += "\n";

                    char **alias = host->h_aliases;
                    if (alias[0] == NULL)
                    {
                        str += "Alias name not found.\n";
                    }
                    else
                    {
                        str += "Alias name:\n";
                        for (int i = 0; alias[i] != NULL; ++i)
                        {
                            str += alias[i];
                            str += "\n";
                        }
                    }
                }
                else
                {
                    str += "-Not found information.\ngethostbyaddr() error: " + std::to_string(WSAGetLastError()) + "\n";
                }
            }

            else
            // Resolve domain name
            {
                printf("Request is domain name.\n");

                addrinfo *result;
                sockaddr_in *address;
                addrinfo hints;
                memset(&hints, 0, sizeof(hints));

                hints.ai_family = AF_INET;
                int rc = getaddrinfo(buff, NULL, &hints, &result);

                // Get the address info
                char ipStr[INET_ADDRSTRLEN];
                if (rc == 0)
                {
                    str += "+";
                    bool IPAddrIsOfficial = true;
                    do
                    {
                        address = (struct sockaddr_in *)result->ai_addr;
                        inet_ntop(AF_INET, &address->sin_addr, ipStr, sizeof(ipStr));
                        if (IPAddrIsOfficial)
                        {
                            str += "Official IP: ";
                            str += ipStr;
                            str += "\n";
                            IPAddrIsOfficial = false;
                            if (result->ai_next == nullptr)
                            {
                                str += "Alias IP not found.\n";
                            }
                            else
                            {
                                str += "Alias IP:\n";
                            }
                        }
                        else
                        {
                            str += ipStr;
                            str += "\n";
                        }
                        result = result->ai_next;
                    } while (result != nullptr);
                }
                else
                {
                    str += "-Not found information.\ngetaddrinfo() error: " + std::to_string(WSAGetLastError()) + "\n";
                }

                freeaddrinfo(result);
            }

            const char *res = str.c_str(); // strcat_s() produces unexpected error, so I must use C++ instead
            ret = sendto(server, res, strlen(res), 0, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
            if (ret == SOCKET_ERROR)
            {
                printf("Error %d: cannot send data.\n", WSAGetLastError());
            }
        }
    } // end while

    // Step 5: Close socket
    closesocket(server);

    // Step 6: Terminate Winsock
    WSACleanup();

    return 0;
}
