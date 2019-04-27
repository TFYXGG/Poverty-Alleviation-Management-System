#pragma once
#define NDEBUG
#include "IdataBase.h"
#include <stdio.h>
#include <assert.h>

#ifdef WINDOWS
#include <Windows.h>
#include <WinSock.h>
#endif // WINDOWS

#include <sql.h>
#include <sqlext.h>


class Database :public IdataBase
{
public:
	//�������ݿ����ӣ�����������Դ���� �û��� ����
	Database(string serverName,string userName,string passWorld);
	vector<vector<string>> query(string sql);
	int upData(string sql);
	~Database();
private:
	HENV henv;
	SQLHDBC hdbc;
};