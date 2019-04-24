
#include "logic.h"
#include<sstream>

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
	ss << "select * from " << " 日汇总 ";
	return db->query(ss.str());
}

bool logic::addAttractions(string name, float fare, string address)
{
	stringstream ss;
	ss << "insert into 景点信息" << " values(" << "'" << name << " '," << fare << "," << "'" << address << "')";
	if (db->upData(ss.str()) != -1)
		return true;
	return false;
}

bool logic::modify(int id, string newName, float newFare, string newAddress)
{	
	stringstream ss;
	ss << "update " << "景点信息" << "set";
	ss << " 名称 " << " = '" << newName << "'," << " 票价 " << " = " << newFare << " , " << " 地区 " << " = '" << newAddress << "'";
	ss << " where " << " id " << " = " << id;
	if (db->upData(ss.str()) != -1)
		return true;
	return false;
}

bool logic::remove(int id)
{
		stringstream ss;
		ss << "delete from " << "景点信息" << " where " << " id " << " = " << id;
		if (db->upData(ss.str()) != -1)
			return true;
		return false;
}

vector<vector<string>> logic::monthlySummary(string month)
{
	return vector<vector<string>>();
}

logic::~logic()
{
	delete db;
}
