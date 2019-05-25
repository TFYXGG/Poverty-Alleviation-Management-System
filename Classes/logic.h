#pragma once
#include "IdataBase.h"

class logic
{
public:
	//获取实例
	logic(IdataBase *db);

	//登陆	登陆名 登陆密码
	bool land(const string &userName, const string &passWorld);

	//景点查询
	vector<vector<string>> sightseeing();

	//增加景点	景点名 票价 地址
	bool addAttractions(const string &name, float fare, const string &address);

	//修改景点信息
	bool modify(int id, const string &newName, float newFare, const string &newAddress);

	//删除景点
	bool remove(int id);

	//总概况
	vector<vector<string>> summary(int year = 0, int month = 0, int day = 0);

	//景点信息
	vector<vector<string>> attractionsInformation();

	~logic();
private:
	IdataBase *db;
};