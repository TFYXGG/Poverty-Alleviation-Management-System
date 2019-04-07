#pragma once

#include "httpMessage.h"
#include "socket.h"


class MessageHandle
{
public:
	//用套接字创建对象
	MessageHandle(ServerSocket *ss);
	//创建处理port端口的对象
	MessageHandle(int port = 80);
	//析构
	~MessageHandle();
private:
	ServerSocket *ss;
};

class HandleThread
{
public:
	//创建消息处理线程对象
	HandleThread(Socket *s);
	//析构
	~HandleThread();
	//运行
	void run();
	//http响应
	void onHttp();
	//发送响应报文
	void sendResponseMessage();
private:
	Socket *s;
	RequestMessage *pRequest;
	ResponseMessage *pResponse;
};