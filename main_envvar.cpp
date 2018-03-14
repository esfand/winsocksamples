
#ifndef UNICODE
#define UNICODE
#endif UNICODE

#include <windows.h>
#ifdef GetEnvironmentStrings
#undef GetEnvironmentStrings
#endif

#include <strsafe.h>


int main_envvar()
{
	char   *lpszVariable;
	char   *lpvEnv;
	char    lpPrefix[8];
	size_t  varsize;

	// Get a pointer to the environment block. 
	lpvEnv = GetEnvironmentStrings();

	// If the returned pointer is NULL, exit.
	if (lpvEnv == NULL)
	{
		wprintf_s(L"GetEnvironmentStrings failed (%d)\n", GetLastError());
		return 0;
	}

	// Variable strings are separated by NULL byte, and the block is 
	// terminated by a NULL byte. 
	lpszVariable = lpvEnv;
	while (*lpszVariable)
	{
		StringCchLengthA(lpszVariable, 2000, &varsize);

		StringCchCopyNA(lpPrefix, 7, lpszVariable, varsize);

		if (true) {
			wprintf_s(L"%hs\n", lpszVariable);
		}

		lpszVariable += varsize/*lstrlen(lpszVariable)*/ + 1;
	}

	FreeEnvironmentStringsA(lpvEnv);

	return 1;
}

