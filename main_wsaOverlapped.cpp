
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Mstcpip.h>

#include <stdio.h>
#include <stdlib.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


int __cdecl main_wsaOverlapped()
{

	//---------------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	WSABUF  DataBuf;

	WSAOVERLAPPED Overlapped;
	SOCKET SendToSocket = INVALID_SOCKET;

	SOCKADDR_IN RecvAddr;
	SOCKADDR_IN LocalAddr;
	int RecvAddrSize  = sizeof(RecvAddr);
	int LocalAddrSize = sizeof(LocalAddr);

	u_short Port = 27015;
	struct hostent *localHost;
	char *ip;

	char SendBuf[1024] = "Data buffer to send";
	int   BufLen    = 1024;
	DWORD BytesSent = 0;
	DWORD Flags     = 0;

	int rc, err;
	int retval = 0;

	//---------------------------------------------
	// Initialize Winsock
	// Load Winsock
	rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (rc != 0) {
		printf("Unable to load Winsock: %d\n", rc);
		return 1;
	}

	// Make sure the SendOverlapped struct is zeroed out
	SecureZeroMemory((PVOID)&Overlapped, sizeof(WSAOVERLAPPED));

	// Create an event handle and setup the overlapped structure.
	Overlapped.hEvent = WSACreateEvent();
	if (Overlapped.hEvent == NULL) {
		printf("WSACreateEvent failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Create a socket for sending data
	SendToSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (SendToSocket == INVALID_SOCKET) {
		printf("socket failed with error: %d\n", WSAGetLastError());
		WSACloseEvent(Overlapped.hEvent);
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "123.123.123.1")
	// and the specified port number.
	RecvAddr.sin_family      = AF_INET;
	RecvAddr.sin_port        = htons(Port);
  //RecvAddr.sin_addr.s_addr = inet_addr("123.123.123.1");
	retval = InetPton(AF_INET, L"123.123.123.1", &RecvAddr.sin_addr.s_addr);

	//---------------------------------------------
	// Set up the LocalAddr structure with the local IP address
	// and the specified port number.
	localHost = gethostbyname("");
	ADDRINFOW hints;
	ADDRINFOW *pResult;
	GetAddrInfoW(L"nodename", L"serviceName", &hints, &pResult);
	ip = inet_ntoa(*(struct in_addr *) *localHost->h_addr_list);

	LocalAddr.sin_family      = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(ip);
	LocalAddr.sin_port        = htons(Port);

	//---------------------------------------------
	// Bind the sending socket to the LocalAddr structure
	// that has the internet address family, local IP address
	// and specified port number.  
	rc = bind(SendToSocket, (struct sockaddr *) &LocalAddr, LocalAddrSize);
	if (rc == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		WSACloseEvent(Overlapped.hEvent);
		closesocket(SendToSocket);
		WSACleanup();
		return 1;
	}
	//---------------------------------------------
	// Send a datagram to the receiver
	printf("Sending a datagram...\n");
	DataBuf.len = BufLen;
	DataBuf.buf = SendBuf;
	rc = WSASendTo(SendToSocket, &DataBuf, 1, &BytesSent, Flags, (SOCKADDR *)& RecvAddr,
                   RecvAddrSize, &Overlapped, NULL);

	if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != (err = WSAGetLastError()))) {
		printf("WSASendTo failed with error: %d\n", err);
		WSACloseEvent(Overlapped.hEvent);
		closesocket(SendToSocket);
		WSACleanup();
		return 1;
	}

	rc = WSAWaitForMultipleEvents(1, &Overlapped.hEvent, TRUE, INFINITE, TRUE);
	if (rc == WSA_WAIT_FAILED) {
		printf("WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
		retval = 1;
	}

	rc = WSAGetOverlappedResult(SendToSocket, &Overlapped, &BytesSent, FALSE, &Flags);
	if (rc == FALSE) {
		printf("WSASendTo failed with error: %d\n", WSAGetLastError());
		retval = 1;
	}

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	printf("Finished sending. Closing socket.\n");
	WSACloseEvent(Overlapped.hEvent);
	closesocket(SendToSocket);
	printf("Exiting.\n");

	//---------------------------------------------
	// Clean up and quit.
	WSACleanup();
	return (retval);
}
