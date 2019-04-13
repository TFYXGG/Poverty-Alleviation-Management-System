
#pragma once
#include "util.h"
#include <WinSock2.h>
#include <stdio.h>
#include <vector>
#include <string>

#pragma comment (lib,"ws2_32.lib")



class Socket
{
public:
	Socket(SOCKET sClient);
	int sendData(const char* Data, int size);	//发送长度为size的Data
	int recvData(char* Data, int size_max);		//接受数据到Data最大长度为size_max
	void close();								//关闭
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
	void close();		//关闭
private:
	SOCKET slisten;
};

