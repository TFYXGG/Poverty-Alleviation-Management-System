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
		//cout << "�߳�:" << this_thread::get_id() << "�յ���һ��" << method << "����" << endl;
		//cout << "URL:" << pRequest->getURL() << endl;
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

#include "Database.h"
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

void main()
{
	srand(time(0));
	Database db("fpglxt", "sa", "1019334418Zz");
	for (int id = 3; id <= 15; id++)
	{
		for (int i = 0; i < 200; i++)
		{
			stringstream ss;
			ss << "insert into �ռ�¼�� values(" << id << ",";
			int quanpiao = rand() % 100;
			int banpiao = rand() % 100;
			ss << (quanpiao + banpiao) << "," << quanpiao << "," << banpiao << "," << rand() % 9000 + 1000<<",'";
			ss << rand() % 5 + 2015 << "-" << rand() % 12 + 1 << "-" << rand() % 31 + 1<<"')";
			int n = db.upData(ss.str());
		}
	}
}
