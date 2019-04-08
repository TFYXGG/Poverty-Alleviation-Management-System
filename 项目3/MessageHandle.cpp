#include "MessageHandle.h"
#include <thread>
#include <sstream>

//缓冲区最大长度
const unsigned int BufSizeMax = 2048;


void run(Socket *s)
{
	HandleThread ht(s);
	ht.run();
}

MessageHandle::MessageHandle(ServerSocket * ss)
{
	this->ss = ss;
	while (true)
	{
		Socket *s = ss->Accept();
		std::thread t(run, s);
		t.detach();
		std::cout << "启动一个线程 ID:" << t.get_id() << std::endl;
	}
}

MessageHandle::MessageHandle(int port)
{
	ss = new ServerSocket(port);
	while (true)
	{
		Socket *s = ss->Accept();
		std::thread t(run, s);
		std::cout << "启动一个线程 ID:" << t.get_id() << std::endl;
		t.detach();
	}
}

MessageHandle::~MessageHandle()
{
	this->ss->close();
	delete ss;
}


HandleThread::HandleThread(Socket * s):pRequest(nullptr), pResponse(nullptr)
{
	this->s = s;
}

HandleThread::~HandleThread()
{
	if (s != nullptr)
	{
		this->s->close();
		delete s;
		s = nullptr;
	}
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
	int len = s->recvData(buf, BufSizeMax);
	pRequest = new RequestMessage(buf, len);
	pResponse = new ResponseMessage();
	onHttp();
	if (!compareNoCase("Keep-Alive", pRequest->getHeaders("Connection")))
		pResponse->setHeaders("Connection", "close");
	else
		pResponse->setHeaders("Connection", "Keep-Alive");
	sendResponseMessage();
}

void HandleThread::onHttp()
{
	//设置根目录
	std::string root = "C:\\Users\\86157\\Desktop\\登陆页面";
	//获取请求方式
	std::string method = pRequest->getMethod();
	if (method == "GET")
	{
		//对GET请求的处理
		std::string URL = pRequest->getURL();
		if (URL == "/")	//请求主页
		{
			pResponse->openFileSetBody(root + "\\enter.html");
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setVersion("HTTP/1.1");
			pResponse->setHeaders("Content-Type", "text/html");
		}
		else
		{
			pResponse->openFileSetBody(root + URL);
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setVersion("HTTP/1.1");
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
