
#ifndef UNICODE
#define UNICODE 1
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <objbase.h>
#include <stdio.h>

// Link with ws2_32.lib and ole32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "ole32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
// Note: could also use malloc() and free()

int main_wsaEnumNameSpaceProviders()
{

	//-----------------------------------------
	// Declare and initialize variables
	WSADATA wsaData;
	int iResult;
	int iError = 0;

	INT iNuminfo = 0;

	int i;


	// Allocate a 4K buffer to retrieve all the namespace providers
	DWORD dwInitialBufferLen = 4096;
	DWORD dwBufferLen;
	LPWSANAMESPACE_INFO lpProviderInfo;


	// variables needed for converting provider GUID to a string
	int iRet = 0;
	WCHAR GuidString[40] = { 0 };

	// Set dwBufferLen to the initial buffer length
	dwBufferLen = dwInitialBufferLen;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}

	lpProviderInfo = (LPWSANAMESPACE_INFO)MALLOC(dwBufferLen);
	if (lpProviderInfo == NULL) {
		wprintf(L"Memory allocation for providers buffer failed\n");
		WSACleanup();
		return 1;
	}

	iNuminfo = WSAEnumNameSpaceProviders(&dwBufferLen, lpProviderInfo);
	if (iNuminfo == SOCKET_ERROR) {
		iError = WSAGetLastError();
		if (iError == WSAEFAULT && dwBufferLen != dwInitialBufferLen) {
			wprintf(L"WSAEnumNameSpaceProviders failed with too small a buffer\n");
			wprintf(L"  Increasing the buffer to %u\n\n", dwBufferLen);
			if (lpProviderInfo) {
				FREE(lpProviderInfo);
				lpProviderInfo = NULL;
			}

			lpProviderInfo = (LPWSANAMESPACE_INFO)MALLOC(dwBufferLen);
			if (lpProviderInfo == NULL) {
				wprintf(L"Memory allocation for providers buffer failed\n");
				WSACleanup();
				return 1;
			}

			iNuminfo = WSAEnumNameSpaceProviders(&dwBufferLen, lpProviderInfo);
			if (iNuminfo == SOCKET_ERROR) {
				wprintf(L"WSAEnumNameSpaceProviders failed with error: %d\n",
					WSAGetLastError());
				if (lpProviderInfo) {
					FREE(lpProviderInfo);
					lpProviderInfo = NULL;
				}
				WSACleanup();
				return 1;
			}
			else
				wprintf(L"\n");
		}
		else {
			wprintf(L"WSAEnumNameSpaceProviders failed with error: %d\n",
				WSAGetLastError());
			if (lpProviderInfo) {
				FREE(lpProviderInfo);
				lpProviderInfo = NULL;
			}
			WSACleanup();
			return 1;
		}
	}

	wprintf(L"WSAEnumNameSpaceProviders succeeded with provider data count = %d\n\n",
		iNuminfo);
	for (i = 0; i < iNuminfo; i++) {
		iRet = StringFromGUID2(lpProviderInfo[i].NSProviderId, (LPOLESTR)&GuidString, 39);
		if (iRet == 0)
			wprintf(L"StringFromGUID2 failed\n");
		else
			wprintf(L"NameSpace ProviderId[%u] = %ws\n", i, GuidString);

		wprintf(L"NameSpace[%u] = ", i);
		switch (lpProviderInfo[i].dwNameSpace) {
		case NS_DNS:
			wprintf(L"Domain Name System (NS_DNS)\n");
			break;
		case NS_WINS:
			wprintf(L"Windows Internet Naming Service (NS_WINS)\n");
			break;
		case NS_NETBT:
			wprintf(L"NetBIOS (NS_NETBT)\n");
			break;
		case NS_NTDS:
			wprintf(L"Windows NT Directory Services (NS_NTDS)\n");
			break;
		case NS_NLA:
			wprintf(L"Network Location Awareness (NS_NLA)\n");
			break;
			// following values only defined on Vista and later
#if(_WIN32_WINNT >= 0x0600)
		case NS_BTH:
			wprintf(L"Bluetooth (NS_BTH)\n");
			break;
		case NS_EMAIL:
			wprintf(L"Email (NS_EMAIL)\n");
			break;
		case NS_PNRPNAME:
			wprintf(L"Peer-to-peer (NS_PNRPNAME)\n");
			break;
		case NS_PNRPCLOUD:
			wprintf(L"Peer-to-peer collection (NS_PNRPCLOUD)\n");
			break;
#endif
		default:
			wprintf(L"Other value (%u)\n", lpProviderInfo[i].dwNameSpace);
			break;
		}

		if (lpProviderInfo[i].fActive)
			wprintf(L"Namespace[%u] is active\n", i);
		else
			wprintf(L"Namespace[%u] is inactive\n", i);

		wprintf(L"NameSpace Version[%u] = %u\n", i, lpProviderInfo[i].dwVersion);

		wprintf(L"Namespace Identifier[%u] = %ws\n\n", i, lpProviderInfo[i].lpszIdentifier);
	}

	if (lpProviderInfo) {
		FREE(lpProviderInfo);
		lpProviderInfo = NULL;
	}
	WSACleanup();

	return 0;
}
