// 
// Connect to a server using its IPv4 addresses 
//
// Making a call to GetAddrInfoEx with an addrinfoex4 structure to retrieve the handle to a FQDN.
//the sample then call WSAIoctl with the ASSOCIATE_NAMERES_CONTEXT_INPUT structure.

#include "winsock2.h"
#include "mstcpip.h"
#include "Ws2tcpip.h"

//#include <stdlib.h>
//#include <stdio.h>
#include <strsafe.h>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")
#pragma comment (lib, "advapi32.lib")

static int myprintf(const wchar_t *lpFormat, ...);
int ConnectServer(const wchar_t *server);

int main_addrinfoex4() {

	return ConnectServer(L"api.twitter.com");

}



int ConnectServer(const wchar_t *server)
{
	int iResult;

	PADDRINFOEX4 pResult = NULL;
	 ADDRINFOEX3 hints   = { 0 };
	PADDRINFOEX4 pCur    = NULL;

	WSADATA wsaData;
	SOCKET connectSocket = INVALID_SOCKET;
	ULONG bytesReturned  = 0;
	ASSOCIATE_NAMERES_CONTEXT_INPUT input = { 0 };
	SOCKADDR_IN clientService;
	DWORD dwRetval;
	//  
	//  Initialize Winsock 
	// 
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		myprintf(L"WSAStartup failed: %d\n", iResult);
		goto Exit;
	}

	//  
	// Create a SOCKET for connection 
	// 
	connectSocket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
	if (connectSocket == INVALID_SOCKET)
	{
		myprintf(L"socket failed: %d\n", WSAGetLastError());
		goto Exit;
	}

	// 
	// Do name resolution 
	// 

	hints.ai_family   = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_EXTENDED | AI_FQDN | AI_CANONNAME | AI_RESOLUTION_HANDLE;
	hints.ai_version  = ADDRINFOEX_VERSION_4;

	dwRetval = GetAddrInfoExW(
		server,
		NULL,
		NS_DNS,
		NULL,
		(const ADDRINFOEXW*)&hints,
		(PADDRINFOEXW*)&pResult,
		NULL,
		NULL,
		NULL, NULL);
	if (dwRetval != 0) {
		myprintf(L"GetAddrInfoEx failed with error: %d\n", dwRetval);
		goto Exit;
	}
	input.TransportSettingId.Guid = ASSOCIATE_NAMERES_CONTEXT;
	input.Handle = (UINT64)HandleToHandle64(pResult->ai_resolutionhandle);

	// 
	// Associate socket with the handle 
	// 

	if (WSAIoctl(
		connectSocket,
		SIO_APPLY_TRANSPORT_SETTING,
		(VOID *)&input,
		sizeof(input),
		NULL,
		0,
		&bytesReturned,
		NULL,
		NULL) == SOCKET_ERROR)
	{
		if (iResult != 0) {
			myprintf(L"WSAIoctl failed: %d\n", WSAGetLastError());
			goto Exit;
		}
	}

	// 
	// Connect to server 
	// 

	pCur = pResult;
	while (pCur != NULL)
	{
		if (pCur->ai_addr->sa_family == AF_INET)
		{
			clientService = *(const sockaddr_in*)pCur->ai_addr;
			clientService.sin_port = htons(80);
			if (connect(
				connectSocket,
				(const SOCKADDR *)&clientService,
				sizeof(clientService)) == SOCKET_ERROR)
			{
				myprintf(L"connect failed: %d\n", WSAGetLastError());
				goto Exit;
			}
			break;
		}
		pCur = pCur->ai_next;
	}

Exit:

	if (connectSocket != INVALID_SOCKET)
	{
		closesocket(connectSocket);
	}
	if (pResult)
	{
		FreeAddrInfoExW((ADDRINFOEXW*)pResult);
	}
	WSACleanup();
	return 0;
}

static int myprintf(const wchar_t *lpFormat, ...) {

	int nLen = 0;
	int nRet = 0;
	wchar_t cBuffer[512] = { '\0' };
	va_list arglist;
	HANDLE hOut = NULL;
	HRESULT hRet;

	ZeroMemory(cBuffer, sizeof(cBuffer));

	va_start(arglist, lpFormat);

	nLen = lstrlen(lpFormat);

	hRet = StringCchVPrintf(cBuffer, 512, lpFormat, arglist);

	if (nRet >= nLen || GetLastError() == 0) {
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOut != INVALID_HANDLE_VALUE)
			WriteConsole(hOut, cBuffer, lstrlen(cBuffer), (LPDWORD)&nLen, NULL);
	}

	return nLen;
}
