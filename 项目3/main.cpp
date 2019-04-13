#include <iostream>
#include "MessageHandle.hpp"

using namespace std;

class PovertyAlleviationManagementSystem:public HandleThread
{
public:
	PovertyAlleviationManagementSystem(Socket *s) :HandleThread(s) {	}
	bool onHttp()
	{
		std::string method = pRequest->getMethod();
		cout << "�߳�:" << this_thread::get_id() << "�յ���һ��" << method << "����" << endl;
		cout << "URL:" << pRequest->getURL() << endl;
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
				pResponse->setHeaders("Content-Type", "text/html");
				return true;
			}
		}
		return false;
	}
};

void main()
{
	PovertyAlleviationManagementSystem::setRoot(R"(��½ҳ��)");
	MessageHandle<PovertyAlleviationManagementSystem> mh(80);
	mh();
}