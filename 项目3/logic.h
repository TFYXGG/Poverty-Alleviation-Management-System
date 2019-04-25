#pragma once

#include "IdataBase.h"

class logic
{
public:
	//获取实例
	logic(IdataBase *db);

	//登陆	登陆名 登陆密码
	bool land(string userName, string passWorld);

	//景点查询
	vector<vector<string>> sightseeing();

	//增加景点	景点名 票价 地址
	bool addAttractions(string name, float fare, string address);

	//修改景点信息
	bool modify(int id, string newName, float newFare, string newAddress);

	//删除景点
	bool remove(int id);

	//总概况
	vector<vector<string>> summary(int year = 0, int month = 0);

	~logic();
private:
	IdataBase *db;
};