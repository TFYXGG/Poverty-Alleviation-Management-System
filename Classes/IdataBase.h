#pragma once
#include <string>
#include <vector>

using namespace std;

class IdataBase
{
public:
	//ִ��sql��䲢����ִ�н��
	virtual vector<vector<string>> query(string const &sql) = 0;
	//ִ�и�����sql��䣬ʧ�ܷ���-1 �ɹ�����Ӱ�������
	virtual int upData(string const &sql) = 0;
	//�ͷ������ݿ����ӣ���������ڴ�
	virtual ~IdataBase() {};
};