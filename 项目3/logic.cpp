
#include "logic.h"


logic::logic(IdataBase * db)
{
	this->db = db;
}

bool logic::land(string nuerName, string passWorld)
{
	return false;
}

vector<vector<string>> logic::sightseeing()
{
	return vector<vector<string>>();
}

bool logic::addAttractions(string name, float fare, string address)
{
	return false;
}

bool logic::modify(int id, string newName, float newFare, string newAddress)
{
	return false;
}

bool logic::remove(int id)
{
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
