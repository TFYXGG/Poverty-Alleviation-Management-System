#pragma once
#include "IdataBase.h"

class logic
{
public:

	//��½	��½�� ��½����
	static bool land(const string &userName, const string &passWorld);

	//�����ѯ
	static vector<vector<string>> sightseeing();

	//���Ӿ���	������ Ʊ�� ��ַ
	static bool addAttractions(const string &name, float fare, const string &address);

	//�޸ľ�����Ϣ
	static bool modify(int id, const string &newName, float newFare, const string &newAddress);

	//ɾ������
	static bool remove(int id);

	//�ܸſ�
	static vector<vector<string>> summary(int year = 0, int month = 0, int day = 0);

	//������Ϣ
	static vector<vector<string>> attractionsInformation();

	//�������ݿ�����
	static void setDatabase(IdataBase *db);

	logic() = delete;

	~logic() = delete;
private:
	static IdataBase *db;
};