
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
	ss << " select ���� from " << " ����Ա��Ϣ " << " where  �û��� = " << " '" << userName << "'";
	auto p = db->query(ss.str());
	return p[0][0] == passWorld;
}

vector<vector<string>> logic::sightseeing()
{
	stringstream ss;
	ss << "select * from �ջ��� ORDER BY ����,���� desc";
	return db->query(ss.str());
}

bool logic::addAttractions(string name, float fare, string address)
{
	stringstream ss;
	ss << "insert into ������Ϣ" << " values(" << "'" << name << " '," << fare << "," << "'" << address << "')";
	return db->upData(ss.str()) != -1;
}

bool logic::modify(int id, string newName, float newFare, string newAddress)
{	
	stringstream ss;
	ss << "update " << "������Ϣ " << "set ";
	ss << " ���� " << " = '" << newName << "'," << " Ʊ�� " << " = " << newFare << " , " << " ���� " << " = '" << newAddress << "'";
	ss << " where " << " id " << " = " << id;
	return db->upData(ss.str()) != -1;
}

bool logic::remove(int id)
{
		stringstream ss;
		ss << "delete from " << "������Ϣ" << " where " << " id " << " = " << id;
		return db->upData(ss.str()) != -1;
}

vector<vector<string>> logic::summary(int year, int month)
{
	stringstream ss;
	if (year&&month)
	{
		ss << "select * from �ջ��� where YEAR(����) = " << year << "and MONTH(����) = " << month <<" ORDER BY ����, DAY(����)";
	}
	else if (year)
	{
		ss << "select * from �»��� where �� = " << year << " ORDER BY ��";
	}
	else
	{
		ss << "select * from �����" << " ORDER BY ����, �� DESC";
	}
	return db->query(ss.str());
}

logic::~logic()
{
	delete db;
}
