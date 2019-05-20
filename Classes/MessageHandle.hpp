#pragma once
#include "httpMessage.h"
#include "socket.h"
#include <thread>
#include "util.h"

template <class T>
class MessageHandle
{
public:
	//���׽��ִ�������
	MessageHandle(ServerSocket *ss);
	//��������port�˿ڵĶ���
	MessageHandle(int port = 80);
	//���ص�()������
	void operator()();
	//����
	~MessageHandle();
private:
	ServerSocket *ss;
};
//
class HandleThread
{
public:
	//������Ϣ�����̶߳���
	HandleThread(Socket *s);
	//����
	virtual ~HandleThread();
	//����
	void run();
	//http��Ӧ	�ɹ���Ӧ����true ���򷵻�false
	virtual bool onHttp() = 0;
	//Ĭ��http��Ӧ
	void onDefaultHttp();
	//������Ӧ����
	void sendResponseMessage();
	//����()������
	void operator()();
	//���ø�Ŀ¼
	static void setRoot(std::string);
protected:
	RequestMessage *pRequest;
	ResponseMessage *pResponse;
	static std::string root;	//�ļ���Ŀ¼
private:
	Socket *s;
	static const unsigned int BufSizeMax;	//��������С
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
	//��ʱδʵ�ֳ����ӹ���
	char buf[BufSizeMax];
	s->setRcvTimeO(1000 * 60 *30);		//���ó�ʱʱ��30����
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
				break;
			try
			{
				pResponse = new ResponseMessage();
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
					//��ʱû����Ϣ��
				}
				if (!onHttp())
					onDefaultHttp();
				pResponse->setVersion("HTTP/1.1");
				pResponse->setHeaders("Connection", "Keep-Alive");
				sendResponseMessage();
			}
			catch (...)
			{
				pResponse->setVersion("HTTP/1.1");
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
				sendResponseMessage();
			}
		} while (compareNoCase(pRequest->getHeaders("Connection"), "Keep-Alive"));
	}
	catch (...)
	{
		//��������δ�������쳣
	}
	//�������������ͷţ������޸ģ�
	if (s != nullptr)
	{
		this->s->Close();
		delete s;
		s = nullptr;
	}
}

void HandleThread::onDefaultHttp()
{
	//��ȡ����ʽ
	std::string method = pRequest->getMethod();
	if (method == "GET")
	{
		//��GET����Ĵ���
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
		//��POST������
		pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::not_found));
		pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::not_found));
	}
	else
	{
		//δ������ࡣ����
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