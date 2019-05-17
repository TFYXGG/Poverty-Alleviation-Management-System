
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
	int sendData(const char* Data, int size);	//发送长度为size的Data
	int recvData(char* Data, int size_max);		//接受数据到Data最大长度为size_max
	void Close();								//关闭
	void setRcvTimeO(long long time);			//设置接受超时 单位毫秒
	void setSndTimeO(long long time);			//设置发送超时 单位毫秒
private:
	SOCKET sClient;
};

class ServerSocket
{
public:
	ServerSocket(int port);			//监听port端口
	Socket* Accept();				//返回套接字
	void Close();		//关闭
private:
	SOCKET slisten;
};

