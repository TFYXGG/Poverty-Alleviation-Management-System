#pragma once
#include "IdataBase.h"

class logic
{
public:
	//��ȡʵ��
	logic(IdataBase *db);

	//��½	��½�� ��½����
	bool land(const string &userName, const string &passWorld);

	//�����ѯ
	vector<vector<string>> sightseeing();

	//���Ӿ���	������ Ʊ�� ��ַ
	bool addAttractions(const string &name, float fare, const string &address);

	//�޸ľ�����Ϣ
	bool modify(int id, const string &newName, float newFare, const string &newAddress);

	//ɾ������
	bool remove(int id);

	//�ܸſ�
	vector<vector<string>> summary(int year = 0, int month = 0, int day = 0);

	//������Ϣ
	vector<vector<string>> attractionsInformation();

	~logic();
private:
	IdataBase *db;
};