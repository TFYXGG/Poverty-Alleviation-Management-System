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
	//构建数据库连接，参数：数据源名称 用户名 密码
	Database(string serverName,string userName,string passWorld);
	vector<vector<string>> query(string sql);
	int upData(string sql);
	~Database();
	static void Init();
private:
	static HENV henv;
	SQLHDBC hdbc;
	static bool sign;
};