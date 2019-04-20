#pragma once

#include "IdataBase.h"
#include <stdio.h>
#include <assert.h>
#include <Windows.h>
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
	SQLHSTMT stmt;
};