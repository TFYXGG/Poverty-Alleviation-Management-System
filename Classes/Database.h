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

extern string serverName;
extern string userName;
extern string passWorld;

class Database :public IdataBase
{
public:
	
	vector<vector<string>> query(string const &sql) override;
	int upData(string const &sql) override;
	~Database();
	static Database* getInstance();
	Database(const Database&) = delete;
	Database& operator = (const Database&) = delete;
private:
	//构建数据库连接，参数：数据源名称 用户名 密码
	Database(string const &serverName, string const &userName, string const &passWorld);
	HENV henv;
	SQLHDBC hdbc;
	static Database* instance;
};