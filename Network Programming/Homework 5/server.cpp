// server.cpp : Defines the entry point for the application.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Homework05.h"
#include <stdio.h>
#pragma warning(disable : 4996)
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <process.h>
#include <time.h>

#define WM_SOCKET WM_USER + 1
#define SERVER_PORT 6000
#define SERVER_ADDR "127.0.0.1"
#define MAX_CLIENT 1024
#define BUFF_SIZE 2048
#define RESPCODELEN 3

#pragma comment(lib, "Ws2_32.lib")

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
HWND InitInstance(HINSTANCE, int);
LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);

typedef struct Status
{
	char* clientIP;
	int clientPort;
	char* username;
	bool isLoggedIn;
} Status;

// Initialize value for created Status
void setStatus(Status* status, const char* clientIP, int clientPort)
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


void recordToLog(const char* clientIP, int clientPort, const char* message, const char* returnCode)
{
	FILE* logPtr = fopen("log_20184000.txt", "a");
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
		fputs((const char*)timeStr, logPtr);
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
int handleRequest(char* response, const char* request, Status* status)
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

				FILE* accPtr = fopen("account.txt", "r");
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

SOCKET client[MAX_CLIENT];
SOCKET listenSock;
Status status[MAX_CLIENT];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	HWND serverWindow;

	//Registering the Window Class
	MyRegisterClass(hInstance);

	//Create the window
	if ((serverWindow = InitInstance(hInstance, nCmdShow)) == NULL)
		return FALSE;

	//Initiate WinSock
	WSADATA wsaData;
	WORD wVersion = MAKEWORD(2, 2);
	if (WSAStartup(wVersion, &wsaData))
	{
		MessageBox(serverWindow, L"Winsock 2.2 is not supported.", L"Error!", MB_OK);
		return 0;
	}

	//Construct socket
	listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//requests Windows message-based notification of network events for listenSock
	WSAAsyncSelect(listenSock, serverWindow, WM_SOCKET, FD_ACCEPT | FD_CLOSE | FD_READ);

	//Bind address to socket
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

	if (bind(listenSock, (sockaddr*)&serverAddr, sizeof(serverAddr)))
	{
		MessageBox(serverWindow, L"Cannot associate a local address with server socket.", L"Error!", MB_OK);
	}

	//Listen request from client
	if (listen(listenSock, MAX_CLIENT))
	{
		MessageBox(serverWindow, L"Cannot place server socket in state LISTEN.", L"Error!", MB_OK);
		return 0;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = windowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK05));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"WindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	int i;
	for (i = 0; i < MAX_CLIENT; i++)
		client[i] = 0;
	hWnd = CreateWindow(L"WindowClass", L"WSAAsyncSelect TCP Server", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_SOCKET	- process the events on the sockets
//  WM_DESTROY	- post a quit message and return
//
//

LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	SOCKET connSock;
	sockaddr_in clientAddr;
	int ret, clientAddrLen = sizeof(clientAddr), clientPort, i;
	char clientIP[INET_ADDRSTRLEN], rcvBuff[BUFF_SIZE], sendBuff[BUFF_SIZE];

	switch (message)
	{
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam))
		{
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam)
				{
					closesocket(client[i]);
					client[i] = 0;
					continue;
				}
		}

		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_ACCEPT:
		{
			connSock = accept((SOCKET)wParam, (sockaddr*)&clientAddr, &clientAddrLen);
			if (connSock == INVALID_SOCKET)
			{
				break;
			}
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == 0)
				{
					client[i] = connSock;
					getpeername(connSock, (sockaddr*)&clientAddr, &clientAddrLen);
					inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, sizeof(clientIP));
					clientPort = ntohs(clientAddr.sin_port);
					//requests Windows message-based notification of network events for listenSock
					WSAAsyncSelect(client[i], hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
					setStatus(&status[i], clientIP, clientPort);
					break;
				}
			if (i == MAX_CLIENT)
				MessageBox(hWnd, L"Too many clients!", L"Notice", MB_OK);
		}
		break;

		case FD_READ:
		{
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam)
					break;

			ret = recv(client[i], rcvBuff, BUFF_SIZE, 0);
			if (ret > 0)
			{
				// Handle request and response to client
				rcvBuff[ret] = 0;
				handleRequest(sendBuff, rcvBuff, &status[i]);
				send(client[i], sendBuff, ret, 0);
			}
		}
		break;

		case FD_CLOSE:
		{
			for (i = 0; i < MAX_CLIENT; i++)
				if (client[i] == (SOCKET)wParam)
				{
					closesocket(client[i]);
					client[i] = 0;
					break;
				}
		}
		break;
		}
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		shutdown(listenSock, SD_BOTH);
		closesocket(listenSock);
		WSACleanup();
		return 0;
	}
	break;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		shutdown(listenSock, SD_BOTH);
		closesocket(listenSock);
		WSACleanup();
		return 0;
	}
	break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
