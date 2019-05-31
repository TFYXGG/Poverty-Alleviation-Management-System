#pragma once
#include "IdataBase.h"
#include <stdio.h>

#define NDEBUG
#include <assert.h>

#ifdef WINDOWS
#include <Windows.h>
#include <WinSock.h>
#endif // WINDOWS

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>


class Database :public IdataBase
{
public:
	//�������ݿ����ӣ�����������Դ���� �û��� ����
	Database(string const &serverName,string const &userName,string const &passWorld);
	vector<vector<string>> query(string const &sql) override;
	int upData(string const &sql) override;
	~Database();
	static void Init();
private:
	static HENV henv;
	SQLHDBC hdbc;
	static bool sign;
};