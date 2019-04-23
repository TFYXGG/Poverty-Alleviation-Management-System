#include "logic.h"

Logic::Logic(IdataBase * db)
{
	this->db = db;
}

int Logic::add(vector<string> information)
{
	if (information.empty())
		return -1;
	stringstream ss;
	ss << "insert into " << BasicTable << " values(";
	ss << information.at(0);
	for (int i = 1; i < information.size(); i++)
	{
		ss << "," << information.at(i);
	}
	ss << ")";
	return db->upData(ss.str());
}

int Logic::del(int id)
{
	stringstream ss;
	ss << "delete from " << BasicTable << " where " << DB_BT_id << " " << id;
	return db->upData(ss.str());
}

int Logic::del(string name)
{
	if (name.empty())
		return -1;
	stringstream ss;
	ss << "delete from " << BasicTable << " where " << DB_BT_name << " = " << name;
	return db->upData(ss.str());
}

int Logic::del(vector<int> ids)
{	
	if (ids.empty())
		return -1;
	stringstream ss;
	ss << "delete from " << BasicTable << " where 1=0 ";
	for (int i = 0; i < ids.size(); i++)
	{
		ss << "or " << DB_BT_id << " =" << ids.at(i) << " ";
	}
	return db->upData(ss.str());
}

int Logic::updata(int id, vector<map<string, string>> information)
{
	stringstream ss;
	ss << "update " << BasicTable << " set";
	for (int i = 0; i < information.size(); i++) 
	{
		ss << information[i]["key"] << " = " << information[i]["value"] << ",";
	}
	ss << "where " << DB_BT_id << " = " << id;
	return db->upData(ss.str());
}

vector<vector<string>> Logic::find(vector<map<string, string>> information)
{
	if (information.empty())
		return vector<vector<string>>();
	stringstream ss;
	ss << "select * from " << BasicTable << " where 1=1";
	for (int i = 0; i < information.size(); i++)
	{
		ss << " and " << information[i]["key"] << information[i]["relation"] << information[i]["value"];
	}
	return db->query(ss.str());
}
int Logic::del(vector<string> names)
{
	if (names.empty())
		return -1;
	stringstream ss;
	ss << "delete from " << BasicTable << " where 1=0";
	for (int i = 0; i < names.size(); i++)
	{
		ss << " or " << DB_BT_name << " = " << names.at(i);
	}
	return db->upData(ss.str());
}

vector<vector<string>> Logic::find(string name, int month)
{
	stringstream ss;
	ss << "select * from " << SummaryTable;
	if (name.empty())
		return db->query(ss.str());
	ss << " where " << DB_ST_name << " = " << name;
	if(month = -1)
		return db->query(ss.str());
	ss << " " << DB_ST_month << " = " << month;
	return db->query(ss.str());
}

Logic::~Logic()
{
	delete db;
}
