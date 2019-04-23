#pragma once
#include "Database.h"
#include <map>
#include <sstream>

//����������
const string BasicTable = "TourismPovertyAlleviationFundManagementSystem";
//���ܱ����
const string SummaryTable = "summary";
//���ֶ���
//id
const string DB_BT_id = "id";
//��������
const string DB_BT_name = "spot_name";
//Ʊ��
const string DB_BT_fare = "fare";
//�ο���
const string DB_BT_tourist = "Amount_of_tourists";
//ȫƱ��
const string DB_BT_full = "Full_ticket_num";
//�ۿ�Ʊ��
const string DB_BT_discount = "discount_num";
//����
const string DB_BT_donation = "Total_donation";
//����
const string DB_BT_date = "date";
//������
const string DB_ST_name = "spot_name";
//�·�
const string DB_ST_month = "month";
//�����
const string DB_ST_donation = "contribution_amount";
//�ο�����
const string DB_ST_tourist = "amount_of_tourists";


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

	//�»���
	//ɾ�� ɾ�����������������Ϣ
	int del(string name);
	int del(vector<string> names);
	//���Ҿ�������Ϣ
	//����������Ϣ
	//month = -1ʱ���Ҹþ��������·����� nameΪ��ʱ�������о�����������
	vector<vector<string>> find(string name = "", int month = -1);

private:
	~Logic();
	IdataBase *db;
};