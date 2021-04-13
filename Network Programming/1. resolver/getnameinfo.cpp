#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    sockaddr_in address;
    char hostname[NI_MAXHOST], servInfo[NI_MAXSERV];

    WSADATA wsaData;
    WORD version = MAKEWORD(2, 2);
    WSAStartup(version, &wsaData);

    address.sin_family = AF_INET;
    char IPAddr[50];
    scanf("%s", IPAddr);
    address.sin_addr.s_addr = inet_addr(IPAddr);
    address.sin_port = htons(5500);
    DWORD dwRet = getnameinfo((struct sockaddr *)&address, sizeof(struct sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);

    if (dwRet == 0)
    {
        printf("getnameinfo() returned hostname: %s\n", hostname);
        printf("getnameinfo() returned service name: %s\n", servInfo);
    }
    else
    {
        printf("Error %d: getnameinfo() failed.\n", WSAGetLastError());
    }

    WSACleanup();

    return 0;
}
