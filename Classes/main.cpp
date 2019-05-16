#include <iostream>
#include "MessageHandle.hpp"
#include "logic.h"
#include "Database.h"
#include <sstream>
#include "util.h"
#include <algorithm>
#include "json.h"

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
				Json::json json;
				auto obj = new Json::object;
				auto arr = new Json::array;
				json.setRoot(obj);
				obj->add("x", arr);
				for (int i = 0; i < p.size(); i++)
				{
					auto tobj = new Json::object;
					tobj->add("id", new Json::numVal(p[i][0]));
					tobj->add("name", new Json::strVal((p[i][1])));
					tobj->add("fare", new Json::numVal(p[i][2]));
					tobj->add("address", new Json::strVal((p[i][3])));
					arr->push_back(tobj);
				}
				pResponse->setHeaders("Content-Type", "text/json");
				pResponse->setBody(json.toJsonFile().data(), json.toJsonFile().length());
				return true;
			}
		}
		else if (method == "POST")
		{
			std::string URL = pRequest->getURL();
			if (URL == "/addviewinfo")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				Json::json j(body, "UTF8");
				auto root = j.getRoot();	
				logic lg(new Database(dsnName, userName, passWord));
				float price = 0.0;
				if(lg.addAttractions(((Json::strVal*)(root->at("name")))->getCppString(), ((Json::numVal*)(root->at("price")))->getFloat(), ((Json::strVal*)(root->at("area")))->getCppString()))
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
				Json::json j(body, "UTF8");
				logic lg(new Database(dsnName, userName, passWord));
				if(lg.remove(((Json::numVal *)(j.getRoot()->at("id")))->getInt()))
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
				Json::json j(body, "UTF-8");
				logic lg(new Database(dsnName, userName, passWord));
				if(lg.modify(((Json::numVal*)(j.getRoot()->at("id")))->getInt(), ((Json::strVal*)(j.getRoot()->at("name")))->getCppString(), ((Json::numVal*)(j.getRoot()->at("fare")))->getFloat(), ((Json::strVal*)(j.getRoot()->at("id")))->getCppString()))
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
				Json::json j(body,"UTF8");
				std::string date = ((Json::strVal*)(j.getRoot()->at("date")))->getCppString();
				int year = 0, month = 0, day = 0;
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
				Json::json ResJson;
				Json::object *root = new Json::object;
				Json::array *a = new Json::array;
				ResJson.setRoot(root);
				root->add("x", a);
				for (int i = 0; i < im.size(); i++)
				{
					Json::object *tobj = new Json::object;
					tobj->add("name", new Json::strVal(im[i][0]));
					tobj->add("date", new Json::strVal(im[i][1]));
					tobj->add("fare", new Json::numVal(im[i][2]));
					tobj->add("total_number_of_visitors", new Json::numVal(im[i][3]));
					tobj->add("full_ticket", new Json::numVal(im[i][4]));
					tobj->add("discount", new Json::numVal(im[i][5]));
					tobj->add("donation", new Json::numVal(im[i][6]));
					a->push_back(tobj);
				}
				pResponse->setHeaders("Content-Type", "text/json");
				pResponse->setBody(ResJson.toJsonFile().data(), ResJson.toJsonFile().length());
				pResponse->setStatus(std::to_string(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setPhrase(ResponseMessage::getStatusString(ResponseMessage::HTTPStatusCode::ok));
				pResponse->setVersion("HTTP/1.1");
				return true;
			}
			else if (URL == "/login")
			{
				string body(pRequest->getBody(), atoi(pRequest->getHeaders("Content-Length").data()));
				Json::json j(body, "UTF8");
				logic lg(new Database(dsnName, userName, passWord));
				Json::object *obj;
				Json::json res(obj);
				if (lg.land(((Json::strVal*)(j.getRoot()->at("username")))->getCppString(),((Json::strVal*)(j.getRoot()->at("password")))->getCppString()))
				{
					obj->add("x", new Json::strVal("true"));
					pResponse->setBody(res.toJsonFile().data(), res.toJsonFile().length());
				}
				else
				{
					obj->add("x", new Json::strVal("false"));
					pResponse->setBody(res.toJsonFile().data(), res.toJsonFile().length());
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