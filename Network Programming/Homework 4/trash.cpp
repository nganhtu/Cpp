// Receive message from client
strcpy_s(clientRequest, MAX_MESS_SIZE, "");
bool validRequest = true;
while (1)
{
    ret = recv(client[i], recvBuff, BUFF_SIZE, 0);
    if (ret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAECONNRESET)
        {
            printf("Connection [%s:%d] reset by peer.\n", status[i].clientIP, status[i].clientPort);
        }
        else
        {
            printf("Error %d: Cannot receive data.\n", WSAGetLastError());
        }
        validRequest = false;
        break;
    }
    else if (ret == 0)
    {
        printf("Unexpected error.\n");
        validRequest = false;
        break;
    }
    else
    {
        recvBuff[ret] = 0;
        strcat_s(clientRequest, MAX_MESS_SIZE, recvBuff);
        if (strstr(recvBuff, "\r\n") != NULL)
        {
            break;
        }
    }
}
if (!validRequest)
{
    FD_CLR(client[i], &initfds);
    closesocket(client[i]);
    client[i] = 0;
}
else
{
    clientRequest[strlen(clientRequest) - 2] = '\0'; // Remove delimiter
    printf("Receive from client [%s:%d]: \"%s\"\n", status[i].clientIP, status[i].clientPort, clientRequest);

    // Send message to client
    memcpy(sendBuff, clientRequest, BUFF_SIZE);
    ret = send(client[i], sendBuff, ret, 0);
    if (ret == SOCKET_ERROR)
    {
        printf("Error %d: cannot send data.\n", WSAGetLastError());
    }
}
