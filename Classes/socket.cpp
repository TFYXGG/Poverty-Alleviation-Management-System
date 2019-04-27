
#include "socket.h"

bool InitFlag = false;
bool socketInit()
{
#ifdef WINDOWS
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}
#endif // WINDOWS
	return true;
}

ServerSocket::ServerSocket(int port)
{
	if (!InitFlag)
	{
		if (socketInit())
		{
			InitFlag = true;
		}
	}
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return;
	}
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
#ifdef WINDOWS
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
#endif // WINDOWS
#ifdef LINUX
	sin.sin_addr.s_addr = 0;
#endif // LINUX

	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	if (listen(slisten, 100) == SOCKET_ERROR)	//最大连接数
	{
		printf("listen error !");
		return;
	}
}

Socket * ServerSocket::Accept()
{
	SOCKET sClient;
	SOCKADDR_IN remoteAddr;
#ifdef LINUX
	unsigned int nAddrlen = sizeof(remoteAddr);
#endif // LINUX
#ifdef WINDOWS
	int nAddrlen = sizeof(remoteAddr);
#endif // WINDOWS
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		return nullptr;
	}
	return new Socket(sClient);
}

void ServerSocket::Close()
{
#ifdef WINDOWS
	closesocket(slisten);
#endif WINDOWS
#ifdef LINUX
	close(slisten);
#endif // LINUX
}



Socket::Socket(SOCKET sClient):sClient(sClient)
{
}

int Socket::sendData(const char * Data, int size)
{
	return send(sClient, Data, size, 0);
}

int Socket::recvData(char * Data, int size_max)
{
	int ret = recv(sClient, Data, size_max, 0);
	if (ret > 0)
	{
		Data[ret] = 0x00;
	}
	return ret;
}

void Socket::Close()
{
#ifdef WINDOWS
	closesocket(sClient);
#endif // WINDOWS
#ifdef LINUX
	close(sClient);
	
#endif // LINUX
}

void Socket::setRcvTimeO(long long time)
{
	setsockopt(sClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, sizeof(long long));
}

void Socket::setSndTimeO(long long time)
{
	setsockopt(sClient, SOL_SOCKET, SO_SNDTIMEO, (char *)&time, sizeof(long long));
}
