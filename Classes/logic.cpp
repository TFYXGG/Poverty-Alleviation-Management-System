
#include "logic.h"
#include <sstream>
#include <ctime>

logic::logic(IdataBase * db)
{
	this->db = db;
}

bool logic::land(string userName, string passWorld)
{	
	stringstream ss;
	ss << " select 密码 from " << " 管理员信息 " << " where  用户名 = " << " '" << userName << "'";
	auto p = db->query(ss.str());
	return p[0][0] == passWorld;
}

vector<vector<string>> logic::sightseeing()
{
	stringstream ss;
	ss << "select * from 日汇总 ORDER BY 名称,日期 desc";
	return db->query(ss.str());
}

bool logic::addAttractions(string name, float fare, string address)
{
	stringstream ss;
	ss << "insert into 景点信息" << " values(" << "'" << name << " '," << fare << "," << "'" << address << "')";
	return db->upData(ss.str()) != -1;
}

bool logic::modify(int id, string newName, float newFare, string newAddress)
{	
	stringstream ss;
	ss << "update " << "景点信息 " << "set ";
	ss << " 名称 " << " = '" << newName << "'," << " 票价 " << " = " << newFare << " , " << " 地区 " << " = '" << newAddress << "'";
	ss << " where " << " id " << " = " << id;
	return db->upData(ss.str()) != -1;
}

bool logic::remove(int id)
{
		stringstream ss;
		ss << "delete from " << "景点信息" << " where " << " id " << " = " << id;
		return db->upData(ss.str()) != -1;
}

vector<vector<string>> logic::summary(int year, int month)
{
	stringstream ss;
	if (year&&month)
	{
		ss << "select * from 日汇总 where YEAR(日期) = " << year << "and MONTH(日期) = " << month <<" ORDER BY 名称, DAY(日期)";
	}
	else if (year)
	{
		ss << "select * from 月汇总 where 年 = " << year << " ORDER BY 月";
	}
	else
	{
		ss << "select * from 年汇总" << " ORDER BY 名称, 年 DESC";
	}
	return db->query(ss.str());
}

logic::~logic()
{
	delete db;
}
