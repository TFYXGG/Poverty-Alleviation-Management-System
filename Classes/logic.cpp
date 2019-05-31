
#include "logic.h"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <mutex>

IdataBase *logic::db = nullptr;

bool logic::land(string const &userName, string const &passWorld)
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

bool logic::addAttractions(string const &name, float fare, string const &address)
{
	stringstream ss;
	ss << "insert into ������Ϣ" << " (����,Ʊ��,����) values (" << "'" << name << " '," << fare << "," << "'" << address << "')";
	auto b = db->upData(ss.str());
	return b != -1 && b != 0;
}

bool logic::modify(int id, string const &newName, float newFare, string const &newAddress)
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

vector<vector<string>> logic::summary(int year, int month,int day)
{
	stringstream ss;
	if (!year)
	{
		ss << "select * from �ջ��� order by ����, ���� desc";
	}
	else if (!month)
	{
		ss << "select * from ����� where ���� = '" << year << "'";
	}
	else if (!day)
	{
		ss << "select * from �»��� where ���� = '" << year << "-" << setw(2) << setfill('0') << month << "'";
	}
	else
	{
		ss << "select * from �ջ��� where ���� = '" << year << "-" << setw(2) << setfill('0') << month << "-" << setw(2) << setfill('0') << day << "'";
	}
	return db->query(ss.str());
}

vector<vector<string>> logic::attractionsInformation()
{
	return db->query("select * from ������Ϣ");
}

void logic::setDatabase(IdataBase * db)
{
	logic::db = db;
}
