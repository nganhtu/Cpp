#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    addrinfo *result; // pointer to the linked-list containing information about the host
    sockaddr_in *address;
    addrinfo hints; // pointer to the linked-list
    memset(&hints, 0, sizeof(hints));

    WSADATA wsaData;
    WORD version = MAKEWORD(2, 2);
    WSAStartup(version, &wsaData);

    hints.ai_family = AF_INET; // only focus on IPv4 address
    char domainName[50];
    scanf("%s", domainName);
    int rc = getaddrinfo(domainName, NULL, &hints, &result);

    // Get the address info
    char ipStr[INET_ADDRSTRLEN];
    if (rc == 0)
    {
        do
        {
            address = (struct sockaddr_in *)result->ai_addr;
            inet_ntop(AF_INET, &address->sin_addr, ipStr, sizeof(ipStr));
            printf("IPv4 address %s\n", ipStr);
            if (result->ai_next == nullptr)
                printf("over.\n");
            result = result->ai_next;
        } while (result != nullptr);
    }
    else
    {
        printf("getaddrinfo() error: %d", WSAGetLastError());
        // thử WSAHOST_NOT_FOUND
    }

    // free linked-list
    freeaddrinfo(result);
    WSACleanup();

    return 0;
}
