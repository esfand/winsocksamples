// WinsockSamples.cpp : Defines the entry point for the console application.
//

int __cdecl main_socket();
int main_setsockopt();
int main_bind();
int main_connect();
int main_listen();
//void ConnectServer(const wchar_t *server);
int main_wsaEnumNameSpaceProviders();
int main_wsaEnumProtocols();
int __cdecl main_wsaOverlapped();
int main_gethostbyname();
int __cdecl main_getaddrinfo();
int __cdecl main_getnameinfo();
int __cdecl main_addrinfo();
int main_addrinfoex4();
int main_envvar();
void main_tracelogging();
int main__rpt();


int main()
{
	main__rpt();
	main_tracelogging();
	main_envvar();
	main_addrinfoex4();
	main_getnameinfo();
	main_getaddrinfo();
	main_gethostbyname();
	main_listen();
	main_socket();
	main_bind();
	main_connect();
	main_setsockopt();

	main_wsaOverlapped();
	main_wsaEnumProtocols();
	main_wsaEnumNameSpaceProviders();

    return 0;
}
