
#pragma once

#ifdef WINDOWS
#include <WinSock2.h>
#endif // WINDOWS

#ifdef LINUX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#endif // LINUX

#include <cstdio>
#include <vector>

#ifdef WINDOWS
#pragma comment (lib,"ws2_32.lib")
#endif // WINDOWS

#ifdef LINUX
typedef int SOCKET;
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
using SOCKADDR_IN = struct sockaddr_in;
using LPSOCKADDR = struct sockaddr *;
using SOCKADDR = struct sockaddr;
#endif // LINUX



class Socket
{
public:
	Socket(SOCKET sClient);
	int sendData(const char* Data, int size);	//���ͳ���Ϊsize��Data
	int recvData(char* Data, int size_max);		//�������ݵ�Data��󳤶�Ϊsize_max
	void Close();								//�ر�
	void setRcvTimeO(long long time);			//���ý��ܳ�ʱ ��λ����
	void setSndTimeO(long long time);			//���÷��ͳ�ʱ ��λ����
private:
	SOCKET sClient;
};

class ServerSocket
{
public:
	ServerSocket(int port);			//����port�˿�
	Socket* Accept();				//�����׽���
	void Close();		//�ر�
private:
	SOCKET slisten;
};

