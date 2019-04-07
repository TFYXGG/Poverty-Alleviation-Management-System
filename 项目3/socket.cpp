#include "socket.h"

bool InitFlag = false;
bool socketInit()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return false;
	}
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
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return;
	}
}

Socket * ServerSocket::Accept()
{
	SOCKET sClient;
	SOCKADDR_IN remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
		return nullptr;
	}
	return new Socket(sClient);
}

void ServerSocket::close()
{
	closesocket(slisten);
}



Socket::Socket(SOCKET sClient) :sClient(sClient)
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

void Socket::close()
{
	closesocket(sClient);
}
