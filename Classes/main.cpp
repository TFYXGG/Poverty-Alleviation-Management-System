#include <iostream>
#include "MessageHandle.hpp"
#include "logic.h"
#include "Database.h"
#include <sstream>
#include "util.h"
#include <algorithm>

using namespace std;

const string dsnName = "fpglxt";
const string userName = "sa";
const string passWord = "1019334418Zz";

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
				pResponse->openFileSetBody(root + "/index.html");
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				pResponse->setHeaders("Content-Type", "text/html");
				return true;
			}
			else if (URL == "/getviewlistinfo")
			{
				logic lg(new Database(dsnName, userName, passWord));
				auto p = lg.attractionsInformation();
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				stringstream ss;
				ss << "{\n\"x\":[";
				for (int i = 0; i < p.size(); i++)
				{
					ss << "{"
						<< "\"id\":" << p[i][0] << ","
						<< "\"name\":" << "\"" << G2U(p[i][1]) << "\""<< ","
						<< "\"fare\":" << p[i][2] << ","
						<< "\"address\":" << "\"" << G2U(p[i][3]) << "\""
						<< "}";
					if (i < p.size() - 1)
					{
						ss << ",\n";
					}
				}
				pResponse->setHeaders("Content-Type", "text/json");
				ss << "]\n}";
				//utf8前缀\xef\xbb\xbf
				pResponse->setBody(("\xef\xbb\xbf"+ss.str()).data(), ("\xef\xbb\xbf" + ss.str()).length());
				return true;
			}
		}
		else if (method == "POST")
		{
			std::string URL = pRequest->getURL();
			if (URL == "/addviewinfo")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				body = U2G(body);
				body.erase(0, 1);
				body.erase(body.length() - 1, 1);
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());	
				auto mv = strToMap(body, ",", ":");	
				logic lg(new Database(dsnName, userName, passWord));
				if (lg.addAttractions(mv.at("name"), atof(mv.at("price").data()), mv.at("area")))
					pResponse->setBody("true", sizeof("true"));
				else
					pResponse->setBody("false", sizeof("false"));
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			else if (URL == "/deleteviewinfo")
			{
				string body(pRequest->getBody(),atoi(pRequest->getHeaders("Content-Length").data()));
				int n = 0;
				body = U2G(body);
				sscanf(body.data(), "{\"id\":%d}", &n);
				logic lg(new Database(dsnName, userName, passWord));
				if(lg.remove(n))
					pResponse->setBody("true", sizeof("true"));
				else
					pResponse->setBody("false", sizeof("false"));
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			else if (URL == "/updateviewinfo")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				body = U2G(body);
				body.erase(0, 1);
				body.erase(body.length() - 1, 1);
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());
				auto mv = strToMap(body, ",", ":");
				logic lg(new Database(dsnName, userName, passWord));
				if(lg.modify(atoi(mv["id"].data()), mv["name"], atof(mv["fare"].data()), mv["address"]))
					pResponse->setBody("true", sizeof("true"));
				else
					pResponse->setBody("false", sizeof("false"));
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			else if (URL == "/getallviewlistinfo")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());
				body.erase(0, 1);
				body.erase(body.length() - 1, 1);
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());
				int year = 0, month = 0, day = 0;
				auto date = split(body, ":").at(1);
				string sYear, sMonth, sDay;
				for (int i = 0; i < date.length() && i < 4; i++)
				{
					sYear += date.at(i);
				}
				for (int i = 4; i < date.length() && i < 6; i++)
				{
					sMonth += date.at(i);
				}
				for (int i = 6; i < date.length() && i < 8; i++)
				{
					sDay += date.at(i);
				}
				if (!sYear.empty())
				{
					year = atoi(sYear.data());
				}
				if (!sMonth.empty())
				{
					month = atoi(sMonth.data());
				}
				if (!sDay.empty())
				{
					day = atoi(sDay.data());
				}
				logic lg(new Database(dsnName, userName, passWord));
				auto im = lg.summary(year, month, day);
				stringstream ss;
				ss << "{\n\"x\":[";
				for (int i = 0; i < im.size(); i++)
				{
					ss << "{"
						<< "\"name\":\"" << G2U(im[i][0]) << "\","
						<< "\"date\":" << "\"" << G2U(im[i][1]) << "\"" << ","
						<< "\"fare\":" << im[i][2] << ","
						<< "\"total_number_of_visitors\":" << im[i][3] << ","
						<< "\"full_ticket\":" << im[i][4]<<","
						<<"\"discount\":"<<im[i][5]<<","
						<<"\"donation\":"<<im[i][6]
						<< "}";
					if (i < im.size() - 1)
					{
						ss << ",\n";
					}
				}
				pResponse->setHeaders("Content-Type", "text/json");
				ss << "]\n}";
				//utf8前缀\xef\xbb\xbf
				pResponse->setBody(("\xef\xbb\xbf" + ss.str()).data(), ("\xef\xbb\xbf" + ss.str()).length());
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			else if (URL == "/login")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());
				body.erase(0, 1);
				body.erase(body.length() - 1, 1);
				body.erase(std::remove(body.begin(), body.end(), '"'), body.end());
				auto user = strToMap(body, ",", ":");
				logic lg(new Database(dsnName, userName, passWord));
				if (lg.land(user["username"], user["password"]))
				{
					string s = "\xef\xbb\xbf{\"x\":\"true\"}";
					pResponse->setBody(s.data(),s.length());
				}
				else
				{
					string s = "\xef\xbb\xbf{\"x\":\"false\"}";
					pResponse->setBody(s.data(), s.length());
				}
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			
		}
		return false;
	}
};


int main()
{
	PovertyAlleviationManagementSystem::setRoot(R"(../Resource/dist)");
	MessageHandle<PovertyAlleviationManagementSystem> mh(3000);
	mh();
	return 0;
}