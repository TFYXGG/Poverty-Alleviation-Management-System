#include "MessageHandle.h"
#include <thread>
#include <sstream>

//��������󳤶�
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
		std::cout << "����һ���߳� ID:" << t.get_id() << std::endl;
	}
}

MessageHandle::MessageHandle(int port)
{
	ss = new ServerSocket(port);
	while (true)
	{
		Socket *s = ss->Accept();
		std::thread t(run, s);
		std::cout << "����һ���߳� ID:" << t.get_id() << std::endl;
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
	s->close();
}

void HandleThread::onHttp()
{
	//���ø�Ŀ¼
	std::string root = "C:\\Users\\86157\\Desktop\\��½ҳ��";
	//��ȡ����ʽ
	std::string method = pRequest->getMethod();
	if (method == "GET")
	{
		//��GET����Ĵ���
		std::string URL = pRequest->getURL();
		if (URL == "/")	//������ҳ
		{
			pResponse->openFileSetBody(root + "\\enter.html");
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setVersion("HTTP/1.1");
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
		//��POST������
	}
	else
	{
		//δ������ࡣ����
	}
}


void HandleThread::sendResponseMessage()
{
	std::string rs;
	std::stringstream rss(rs);
	rss << pResponse->version << " " << pResponse->status << " " << pResponse->reasonPhrase << "\r\n";
	for (std::map<std::string, std::string>::iterator it = pResponse->headers.begin(); it != pResponse->headers.end(); it++)
	{
		rss << it->first << ": " << it->second << "\r\n";
	}
	rss << "\r\n";
	s->sendData(rs.data(), rs.length());
	int len = 0;
	try {
		len = atoi(pResponse->headers.at("Content-Length").data());
		s->sendData(pResponse->entityBody, len);
	}
	catch (std::out_of_range e)
	{
		std::cout << "������Ϣ��ʱ������һ���쳣:" << e.what() << std::endl;
	}
}
