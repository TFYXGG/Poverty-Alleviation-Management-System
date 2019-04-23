#pragma once
#include "Database.h"
#include <map>
#include <sstream>

class Logic
{
public:
	Logic(IdataBase *db);

	//���ز���Ӱ������� �������Ϊ��ֵ������ information[0]{key = name,relation = "=" value = "������"},information[1]{key = address,relation = "=" ,value = "����"}
	int add(vector<string> information);
	//����idɾ��
	int del(int id);
	int del(vector<int> ids);
	//�޸� id���� 
	int updata(int id, vector<map<string, string>> information);
	//����
	vector<vector<string>> find(vector<map<string, string>> information);

private:
	~Logic();
	IdataBase *db;
};