#pragma once
#include "httpMessage.h"
#include "socket.h"
#include <thread>
#include "util.h"

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
	static void setRoot(std::string const &root);
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
	ss->Close();
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
	char buf[BufSizeMax];
	s->setRcvTimeO(1000 * 60 *30);		//设置超时时间30分钟
	try
	{
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
				goto sign;
			pResponse = new ResponseMessage();
			//协议版本
			pResponse->setVersion("HTTP/1.1");
			pRequest = new RequestMessage(buf, len);
			try {
				while (pRequest->getAlreadySavedBodyLength() < atoi(pRequest->getHeaders("Content-Length").data()))
				{
					int len = s->recvData(buf, BufSizeMax);
					if (len <= 0)
						break;
					pRequest->apendBody(buf, len);
				}
			}
			catch (std::out_of_range e)
			{
				//此时没有消息体
			}
			if (!onHttp())
				onDefaultHttp();
			pResponse->setHeaders("Connection", "Keep-Alive");
			if (pRequest == nullptr)
				break;
			sendResponseMessage();
		} while (compareNoCase(pRequest->getHeaders("Connection"), "Keep-Alive"));
	}
	catch (IncompleteMessage e)
	{
		//请求报文不完整
		pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::bad_request));
		pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::bad_request));
	}
	catch (UnableToOpenResource e)
	{
		//无法打开所需要的资源
		pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
		pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
	}
	catch (...)
	{
		//其他的错误
		pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::bad_request));
		pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::bad_request));
	}
	sendResponseMessage();
	//不能在析构中释放（不能修改）
	sign:
	if (s != nullptr)
	{
		this->s->Close();
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
		pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
		pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
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

void HandleThread::setRoot(std::string const &root)
{
	HandleThread::root = root;
}
