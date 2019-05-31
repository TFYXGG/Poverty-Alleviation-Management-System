#pragma once
#include "IdataBase.h"

class logic
{
public:

	//登陆	登陆名 登陆密码
	static bool land(const string &userName, const string &passWorld);

	//景点查询
	static vector<vector<string>> sightseeing();

	//增加景点	景点名 票价 地址
	static bool addAttractions(const string &name, float fare, const string &address);

	//修改景点信息
	static bool modify(int id, const string &newName, float newFare, const string &newAddress);

	//删除景点
	static bool remove(int id);

	//总概况
	static vector<vector<string>> summary(int year = 0, int month = 0, int day = 0);

	//景点信息
	static vector<vector<string>> attractionsInformation();

	//设置数据库连接
	static void setDatabase(IdataBase *db);

	logic() = delete;

	~logic() = delete;
private:
	static IdataBase *db;
};