#pragma once

#include "httpMessage.h"
#include "socket.h"


class MessageHandle
{
public:
	//���׽��ִ�������
	MessageHandle(ServerSocket *ss);
	//��������port�˿ڵĶ���
	MessageHandle(int port = 80);
	//����
	~MessageHandle();
private:
	ServerSocket *ss;
};

class HandleThread
{
public:
	//������Ϣ�����̶߳���
	HandleThread(Socket *s);
	//����
	~HandleThread();
	//����
	void run();
	//http��Ӧ
	void onHttp();
	//������Ӧ����
	void sendResponseMessage();
private:
	Socket *s;
	RequestMessage *pRequest;
	ResponseMessage *pResponse;
};