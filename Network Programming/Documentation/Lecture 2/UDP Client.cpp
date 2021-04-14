#include <iostream>
#include <stdio.h>
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
    SOCKET client;
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client == INVALID_SOCKET)
    {
        std::cout << "Error: " << WSAGetLastError() << std::endl;
    }
    //(optional) Set time-out for receiving
    int tv = 10000; //Time-out interval: 10000ms
    setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, (const char *)(&tv), sizeof(int));

    //Step 3: Specify server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

    //Step 4: Communicate with server
    char buff[BUFF_SIZE];
    int ret, serverAddrLen = sizeof(serverAddr), messageLen;
    while (1)
    {
        //Send message
        std::cout << "Send to server: ";
        fgets(buff, BUFF_SIZE, stdin);
        if (strlen(buff) == 0)
        {
            break;
        }
        // đây rồi, lỗi là input thừa 1 ký tự xuống dòng
        // còn vì sao mà chỉ client cần ip của server,
        // vì bên server thì ip là cố định rồi
        // TODO thử viết 2 server khác ip?
        else
        {
            // hay là dùng gets cho đơn giản? điểm thấp
            printf("%d\n", strlen(buff));
        }

        ret = sendto(client, buff, strlen(buff), 0, (sockaddr *)&serverAddr, serverAddrLen);
        if (ret == SOCKET_ERROR)
            std::cout << "Error: " << WSAGetLastError() << std::endl;

        //Receive echo message
        ret = recvfrom(client, buff, BUFF_SIZE, 0, (sockaddr *)&serverAddr, &serverAddrLen);

        if (ret == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
                std::cout << "Time-out!";
            else
                std::cout << "Error: " << WSAGetLastError() << std::endl;
        }
        else if (strlen(buff) > 0)
        {
            if (ret < BUFF_SIZE)
            {
                buff[ret] = 0;
                std::cout << "Receive from server : " << buff << std::endl;
            }
        }
    }

    //Step 5: Close socket
    closesocket(client);

    //Step 6: Terminate Winsock
    WSACleanup();

    return 0;
}
