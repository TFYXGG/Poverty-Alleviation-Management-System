#include "logic.h"

Logic::Logic(IdataBase * db)
{
	this->db = db;
}

int Logic::add(vector<string> information)
{
	string sql = R"(
	insert
	into 
	TourismPovertyAlleviationFundManagementSystem 
	values()";
	for (int i = 0; i < information.size(); i++)
	{
		sql += information.at(i);
		sql += ",";
	}
	sql += ")";
	return db->upData(sql);
}

int Logic::del(int id)
{
	string sql = R"(
	delete
	from
	TourismPovertyAlleviationFundManagementSystem
	where id=)";
	char buf[256];
	itoa(id, buf, 10);
	sql += buf;
	return db->upData(sql);
}

int Logic::del(vector<int> ids)
{	
	if (ids.empty())
		return -1;
	string sql = R"(
	delete
	from
	TourismPovertyAlleviationFundManagementSystem
	where 1=0)";
	for (int i = 0; i < ids.size(); i++)
	{
		sql += " or";
		sql += " id = ";
		char buf[256];
		itoa(ids[i], buf, 10);
		sql += buf;
	}
	return db->upData(sql);
}

int Logic::updata(int id, vector<map<string, string>> information)
{
	stringstream ss;
	ss << R"(update
		TourismPovertyAlleviationFundManagementSystem
		set)";
//	string sql = R"(
//	update
//	TourismPovertyAlleviationFundManagementSystem
//	set
//)";
	for (int i = 0; i < information.size(); i++) 
	{
		ss << information[i]["key"] << " = " << information[i]["value"] << ",";
		//sql += information[i]["key"];
		//sql += "=";
		//sql += information[i]["value"];
		//sql += ",";
	}
	ss << "where id = " << id;
	/*sql += "where id = ";
	char buf[256];
	
	sql+=*/
	return db->upData(ss.str());
}

vector<vector<string>> Logic::find(vector<map<string, string>> information)
{
	stringstream ss;
	ss << R"(
	select *
	from TourismPovertyAlleviationFundManagementSystem
	where 1=1)";
	for (int i = 0; i < information.size(); i++)
	{
		ss << " and " << information[i]["key"] << information[i]["relation"] << information[i]["value"];
	}
	return db->query(ss.str());
}
