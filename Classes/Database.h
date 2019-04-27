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
	//构建数据库连接，参数：数据源名称 用户名 密码
	Database(string serverName,string userName,string passWorld);
	vector<vector<string>> query(string sql);
	int upData(string sql);
	~Database();
private:
	HENV henv;
	SQLHDBC hdbc;
};