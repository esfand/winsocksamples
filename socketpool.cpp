// socket pooling through winsock
// i need to create a pool of open sockets at application startup, use them and then perform clean - up at shut - down.
// in my sample application main, i invoke the WSAStartup to invoke the winsoch environment and then call all the methods in my socket class in a loop

/*
void main_socketpool()
{

	URLConnect m_URL(2);
	SOCKET sd;

	WORD version = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(version, &wsaData);

	m_URL.HttpConnect();
	char s[256] = "";
	memset(s, ' ', 256);
	strcpy(s, "GET");
	// .
	// .
	char* gotstring = NULL;
	for (int i = 0;i&lt;2;i++)
	{
		m_URL.HttpOpenSession(sd);
		m_URL.HttpSendRequest(sd, s);
		if (m_URL.ReadString(sd, gotstring))
		{
			printf("the string %s from socket %d", gotstring, sd);
			free(gotstring);
		}
	}

	for (i = 0;i&lt;2;i++)
	{
		m_URL.HttpCloseSession(sd);
	}

	m_URL.HttpClose();
	WSACleanup();
}

//the constructor does the space allocation for the socket array

URLConnect::URLConnect(int pSOCK_SIZE)
{
	NO_SOCK = new int(pSOCK_SIZE);
	ActiveSocketList = (SockList**)malloc(pSOCK_SIZE * sizeof(SockList*));
}

// the http connect creates the socket pool

bool URLConnect::HttpConnect()
{

	LPHOSTENT lpHostEntry;
	SOCKADDR_IN saServer;

	lpHostEntry = gethostbyname("mars.wipro.com");

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry - &gt;h_addr_list);
	saServer.sin_port = htons(25554);

	for (count = 0;count&lt;(*tmp);count++)
	{
		ActiveSocketList[count] = (SockList*)malloc(sizeof(struct SockList));
		ActiveSocketList[count] - &gt;fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ((ActiveSocketList[count] - &gt;fd != SOCKET_ERROR))
			nret = connect((ActiveSocketList[count] - &gt;fd), (LPSOCKADDR)&saServer, sizeof(struct sockaddr));
		if (nret == SOCKET_ERROR)
		{
			ret = 0;
			break;
		}
		if (ret)
			return true;
		else return false;
	}
	
	// then i take the socket handles from the list and send request to the server.
	// the problewm is that only the first request reaches the server though 
	// the sendrequest method is not raising any erro for the subsequent calls.
	// is there some mistake in the way i am invoking the winsock environment
	*/