
#include "logic.h"
#include<sstream>

logic::logic(IdataBase * db)
{
	this->db = db;
}

bool logic::land(string userName, string passWorld)
{	
	stringstream ss;
	ss << " select ���� from " << " ����Ա��Ϣ " << " where  �û��� = " << " '" << userName << "'";
	auto p = db->query(ss.str());
	return p[0][0] == passWorld;
}

vector<vector<string>> logic::sightseeing()
{
	stringstream ss;
	ss << "select * from " << " �ջ��� ";
	return db->query(ss.str());
}

bool logic::addAttractions(string name, float fare, string address)
{
	stringstream ss;
	ss << "insert into ������Ϣ" << " values(" << "'" << name << " '," << fare << "," << "'" << address << "')";
	if (db->upData(ss.str()) != -1)
		return true;
	return false;
}

bool logic::modify(int id, string newName, float newFare, string newAddress)
{	
	stringstream ss;
	ss << "update " << "������Ϣ" << "set";
	ss << " ���� " << " = '" << newName << "'," << " Ʊ�� " << " = " << newFare << " , " << " ���� " << " = '" << newAddress << "'";
	ss << " where " << " id " << " = " << id;
	if (db->upData(ss.str()) != -1)
		return true;
	return false;
}

bool logic::remove(int id)
{
		stringstream ss;
		ss << "delete from " << "������Ϣ" << " where " << " id " << " = " << id;
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
