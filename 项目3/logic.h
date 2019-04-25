#pragma once

#include "IdataBase.h"

class logic
{
public:
	//��ȡʵ��
	logic(IdataBase *db);

	//��½	��½�� ��½����
	bool land(string userName, string passWorld);

	//�����ѯ
	vector<vector<string>> sightseeing();

	//���Ӿ���	������ Ʊ�� ��ַ
	bool addAttractions(string name, float fare, string address);

	//�޸ľ�����Ϣ
	bool modify(int id, string newName, float newFare, string newAddress);

	//ɾ������
	bool remove(int id);

	//�ܸſ�
	vector<vector<string>> summary(int year = 0, int month = 0);

	~logic();
private:
	IdataBase *db;
};