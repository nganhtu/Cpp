#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5500
#define BUFF_SIZE 2048
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    //Step 1: Inittiate WinSock
    WSADATA wsaData;
    WORD wVersion = MAKEWORD(2, 2);
    if (WSAStartup(wVersion, &wsaData))
        std::cout << "Version is not supported." << std::endl;

    //Step 2: Construct socket
    SOCKET server;
    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server == INVALID_SOCKET)
    {
        std::cout << "Error cannot create server socket: " << WSAGetLastError() << std::endl;
    }

    //Step 3: Bind address to socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5500);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);
    if (bind(server, (sockaddr *)&serverAddr, sizeof(serverAddr)))
    {
        std::cout << "Error cannot bind this address: " << WSAGetLastError() << std::endl;
        return 0;
    }
    printf("Server started!\n");

    //Step 4: Communicate with client
    sockaddr_in clientAddr;
    char buff[BUFF_SIZE], clientIP[INET_ADDRSTRLEN];
    int ret, clientAddrLen = sizeof(clientAddr), clientPort;
    while (1)
    {
        //Receive message
        ret = recvfrom(server, buff, BUFF_SIZE, 0, (sockaddr *)&clientAddr, &clientAddrLen);
        if (ret == SOCKET_ERROR)
            std::cout << "Error cannot receive data: " << WSAGetLastError() << std::endl;
        else if (strlen(buff) > 0)
        {
            if (ret < BUFF_SIZE)
                buff[ret] = 0;
            else
            {
                std::cout << "BUFF_SIZE is too small." << std::endl;
                return 0;
            }
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
            clientPort = ntohs(clientAddr.sin_port);
            std::cout << "Receive from client " << clientIP << ": " << clientPort << " " << buff << std::endl;
            //Echo to client
            ret = sendto(server, buff, ret, 0, (SOCKADDR *)&clientAddr, sizeof(clientAddr));
            if (ret == SOCKET_ERROR)
                std::cout << "Error: " << WSAGetLastError() << std::endl;
        }
    }

    //Step 5: Close socket
    closesocket(server);

    //Step 6: Terminate Winsock
    WSACleanup();

    return 0;
}
