#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

int main_setsockopt()
{

	//---------------------------------------
	// Declare variables
	WSADATA wsaData;

	SOCKET ListenSocket;
	sockaddr_in service;

	int iResult = 0;

	BOOL bOptVal = FALSE;
	int bOptLen = sizeof(BOOL);

	int iOptVal = 0;
	int iOptLen = sizeof(int);

	//---------------------------------------
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"Error at WSAStartup()\n");
		return 1;
	}
	//---------------------------------------
	// Create a listening socket
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) {
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//---------------------------------------
	// Bind the socket to the local IP address
	// and port 27015
	hostent *thisHost;
	char *ip;
	u_short port;
	port = 27015;
	thisHost = gethostbyname("");
	ip = inet_ntoa(*(struct in_addr *) *thisHost->h_addr_list);

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr(ip);
	service.sin_port = htons(port);

	iResult = bind(ListenSocket, (SOCKADDR *)& service, sizeof(service));
	if (iResult == SOCKET_ERROR) {
		wprintf(L"bind failed with error %u\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}
	//---------------------------------------
	// Initialize variables and call setsockopt. 
	// The SO_KEEPALIVE parameter is a socket option 
	// that makes the socket send keepalive messages
	// on the session. The SO_KEEPALIVE socket option
	// requires a boolean value to be passed to the
	// setsockopt function. If TRUE, the socket is
	// configured to send keepalive messages, if FALSE
	// the socket configured to NOT send keepalive messages.
	// This section of code tests the setsockopt function
	// by checking the status of SO_KEEPALIVE on the socket
	// using the getsockopt function.

	bOptVal = TRUE;

	iResult = getsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&iOptVal, &iOptLen);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
	}
	else
		wprintf(L"SO_KEEPALIVE Value: %ld\n", iOptVal);

	iResult = setsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"setsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
	}
	else
		wprintf(L"Set SO_KEEPALIVE: ON\n");

	iResult = getsockopt(ListenSocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&iOptVal, &iOptLen);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"getsockopt for SO_KEEPALIVE failed with error: %u\n", WSAGetLastError());
	}
	else
		wprintf(L"SO_KEEPALIVE Value: %ld\n", iOptVal);

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}