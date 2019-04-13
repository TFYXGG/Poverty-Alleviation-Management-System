#pragma once

#include "httpMessage.h"
#include "socket.h"
#include <thread>

template <class T>
class MessageHandle
{
public:
	//用套接字创建对象
	MessageHandle(ServerSocket *ss);
	//创建处理port端口的对象
	MessageHandle(int port = 80);
	//重载的()操作符
	void operator()();
	//析构
	~MessageHandle();
private:
	ServerSocket *ss;
};
//
class HandleThread
{
public:
	//创建消息处理线程对象
	HandleThread(Socket *s);
	//析构
	virtual ~HandleThread();
	//运行
	void run();
	//http响应	成功响应返回true 否则返回false
	virtual bool onHttp() = 0;
	//默认http响应
	void onDefaultHttp();
	//发送响应报文
	void sendResponseMessage();
	//重载()操作符
	void operator()();
	//设置根目录
	static void setRoot(std::string);
protected:
	RequestMessage *pRequest;
	ResponseMessage *pResponse;
	static std::string root;	//文件根目录
private:
	Socket *s;
	static const unsigned int BufSizeMax;	//缓冲区大小
};


template <class T>
MessageHandle<T>::MessageHandle(ServerSocket * ss)
{
	this->ss = ss;
}
template <class T>
MessageHandle<T>::MessageHandle(int port)
{
	ss = new ServerSocket(port);
}

template <class T>
void MessageHandle<T>::operator()()
{
	while (true)
	{
		auto s = ss->Accept();
		T thread(s);
		std::thread t(thread);
		t.detach();
	}
}
template <class T>
MessageHandle<T>::~MessageHandle()
{
	ss->close();
	delete ss;
}

std::string HandleThread::root;
const unsigned HandleThread::BufSizeMax = 4096;

HandleThread::HandleThread(Socket * s) :pRequest(nullptr), pResponse(nullptr) ,s(s)
{
}

HandleThread::~HandleThread()
{
	if (pRequest != nullptr)
	{
		delete pRequest;
		pRequest = nullptr;
	}
	if (pResponse != nullptr)
	{
		delete pResponse;
		pResponse = nullptr;
	}
}

void HandleThread::run()
{
	//暂时未实现长链接功能
	char buf[BufSizeMax];
	s->setRcvTimeO(1000 * 60 * 60);		//设置超时时间1小时
	do {
		if (pResponse != nullptr)
		{
			delete pResponse;
			pResponse = nullptr;
		}
		if (pRequest != nullptr)
		{
			delete pRequest;
			pRequest = nullptr;
		}
		int len = s->recvData(buf, BufSizeMax);
		if (len <= 0)
			break;
		try
		{
			pResponse = new ResponseMessage();
			pRequest = new RequestMessage(buf, len);
			if (!onHttp())
				onDefaultHttp();
			pResponse->setVersion("HTTP/1.1");
			pResponse->setHeaders("Connection", "Keep-Alive");
			sendResponseMessage();
		}
		catch (...)
		{
			//std::cout << "无法解析的消息" << std::endl;
			pResponse->setVersion("HTTP/1.1");
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
			sendResponseMessage();
		}
	} while (compareNoCase(pRequest->getHeaders("Connection"), "Keep-Alive"));
	if (s != nullptr)
	{
		this->s->close();
		delete s;
		s = nullptr;
	}
}

void HandleThread::onDefaultHttp()
{
	//获取请求方式
	std::string method = pRequest->getMethod();
	if (method == "GET")
	{
		//对GET请求的处理
		std::string URL = pRequest->getURL();
		if (pResponse->openFileSetBody(root + URL))
		{
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
		}
		else
		{
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
		}
	}
	else if (method == "POST")
	{
		//对POST请求处理
	}
	else
	{
		//未定义更多。。。
	}
}

void HandleThread::sendResponseMessage()
{
	char *buf = nullptr;
	int len = pResponse->getByte(buf);
	s->sendData(buf, len);
	delete[] buf;
}

void HandleThread::operator()()
{
	this->run();
}

void HandleThread::setRoot(std::string root)
{
	HandleThread::root = root;
}
