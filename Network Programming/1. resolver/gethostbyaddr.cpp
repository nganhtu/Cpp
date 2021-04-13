#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    WORD version = MAKEWORD(2, 2);
    WSAStartup(version, &wsaData);

    char IPAddr[20];
    scanf("%s", IPAddr);
    u_long address = inet_addr(IPAddr);
    hostent *host = gethostbyaddr((char *)&address, sizeof(address), AF_INET);
    printf("Host name: %s\n", host->h_name);

    char **alias = host->h_aliases;
    for (int i = 0; alias[i] != NULL; ++i)
    {
        printf("Alias: %s\n", alias[i]);
    }

    WSACleanup();

    return 0;
}
