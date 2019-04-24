#pragma once

#include "IdataBase.h"

class logic
{
public:
	//获取实例
	logic(IdataBase *db);

	//登陆	登陆名 登陆密码
	bool land(string nuerName, string passWorld);

	//景点查询
	vector<vector<string>> sightseeing();

	//增加景点	景点名 票价 地址
	bool addAttractions(string name, float fare, string address);

	//修改景点信息
	bool modify(int id, string newName, float newFare, string newAddress);

	//删除景点
	bool remove(int id);

	//月总概况
	vector<vector<string>> monthlySummary(string month);

	~logic();
private:
	IdataBase *db;
};