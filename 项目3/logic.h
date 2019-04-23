#pragma once
#include "Database.h"
#include <map>
#include <sstream>

//基本表，表名
const string BasicTable = "TourismPovertyAlleviationFundManagementSystem";
//汇总表表名
const string SummaryTable = "summary";
//各字段名
//id
const string DB_BT_id = "id";
//景点名称
const string DB_BT_name = "spot_name";
//票价
const string DB_BT_fare = "fare";
//游客量
const string DB_BT_tourist = "Amount_of_tourists";
//全票量
const string DB_BT_full = "Full_ticket_num";
//折扣票量
const string DB_BT_discount = "discount_num";
//捐款额
const string DB_BT_donation = "Total_donation";
//日期
const string DB_BT_date = "date";
//景点名
const string DB_ST_name = "spot_name";
//月份
const string DB_ST_month = "month";
//捐款金额
const string DB_ST_donation = "contribution_amount";
//游客数量
const string DB_ST_tourist = "amount_of_tourists";


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

	//月汇总
	//删除 删除整个景点的所有信息
	int del(string name);
	int del(vector<string> names);
	//查找景点月信息
	//查找所有信息
	//month = -1时查找该景点所有月份数据 name为空时查找所有景点所有数据
	vector<vector<string>> find(string name = "", int month = -1);

private:
	~Logic();
	IdataBase *db;
};