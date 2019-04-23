#pragma once
#include "Database.h"
#include <map>
#include <sstream>

class Logic
{
public:
	Logic(IdataBase *db);

	//返回操作影响的行数 输入参数为键值对数组 information[0]{key = name,relation = "=" value = "大明宫"},information[1]{key = address,relation = "=" ,value = "西安"}
	int add(vector<string> information);
	//根据id删除
	int del(int id);
	int del(vector<int> ids);
	//修改 id不变 
	int updata(int id, vector<map<string, string>> information);
	//查找
	vector<vector<string>> find(vector<map<string, string>> information);

private:
	~Logic();
	IdataBase *db;
};