
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
	int sendData(const char* Data, int size);	//���ͳ���Ϊsize��Data
	int recvData(char* Data, int size_max);		//�������ݵ�Data��󳤶�Ϊsize_max
	void close();								//�ر�
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
	void close();		//�ر�
private:
	SOCKET slisten;
};

