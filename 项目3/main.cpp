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
		//cout << "线程:" << this_thread::get_id() << "收到了一个" << method << "请求" << endl;
		//cout << "URL:" << pRequest->getURL() << endl;
		if (method == "GET")
		{
			//对GET请求的处理
			std::string URL = pRequest->getURL();
			if (URL == "/")	//请求主页
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
	PovertyAlleviationManagementSystem::setRoot(R"(D:\其他资源\H-ui.admin-master)");
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
//	auto p = db.query("select id from 景点信息");
//	for (int i = 0; i < p.size(); i++)
//	{
//		for (int j = 0; j < 10000; j++)
//		{
//			stringstream ss;
//			ss << "insert into 日记录表 values(" << p[i][0] << "," << rand() % 90000 + 10000 << "," << rand() % 90000 + 10000 << ",'" << rand() % 5 + 2015 << "-" << rand() % 12 + 1 << "-" << rand() % 31 + 1 << "')";
//			int n = db.upData(ss.str());
//		}
//	}
//}