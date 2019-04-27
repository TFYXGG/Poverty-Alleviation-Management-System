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
				pResponse->openFileSetBody(root + "\\admin-list.html");
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				pResponse->setHeaders("Content-Type", "text/html");
				return true;
			}
		}
		else if (method == "POST")
		{
			pResponse->setVersion("HTTP/1.1");
			pResponse->setHeaders("Content-Type", "json");
			pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
			pResponse->setBody("success", 7);
		}
		return false;
	}
};

void main()
{
	PovertyAlleviationManagementSystem::setRoot(R"(D:\������Դ\H-ui.admin-master)");
	MessageHandle<PovertyAlleviationManagementSystem> mh(80);
	mh();
}
//
//#include "Database.h"
//#include <sstream>
//#include <ctime>
//
//
//void main()
//{
//	srand(time(0));
//	Database db("fpglxt", "sa", "1019334418Zz");
//	auto p = db.query("select id from ������Ϣ");
//	for (int i = 0; i < p.size(); i++)
//	{
//		for (int j = 0; j < 10000; j++)
//		{
//			stringstream ss;
//			ss << "insert into �ռ�¼�� values(" << p[i][0] << "," << rand() % 90000 + 10000 << "," << rand() % 90000 + 10000 << ",'" << rand() % 5 + 2015 << "-" << rand() % 12 + 1 << "-" << rand() % 31 + 1 << "')";
//			int n = db.upData(ss.str());
//		}
//	}
//}