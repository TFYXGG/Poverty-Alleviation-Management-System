#include "Database.h"
#include <sstream>
#include <iostream>


Database::Database(string serverName, string userName, string passWorld) :henv(NULL), hdbc(NULL)
{
	SQLRETURN rcode;
	// *) 申请环境句柄
	rcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) 设置ODBC版本的环境属性
	rcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, SQL_IS_INTEGER);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) 分配连接句柄
	rcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) 连接数据源
	rcode = SQLConnect(hdbc, (SQLCHAR *)serverName.data(), serverName.length(),
		(SQLCHAR *)userName.data(), userName.length(), (SQLCHAR *)passWorld.data(), passWorld.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
}

vector<vector<string>> Database::query(string sql)
{
	// *) 分配语句句柄
	SQLHSTMT stmt;
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return vector<vector<string>>();
	}
	//执行具体的sql
	rcode = SQLExecDirect(stmt, (SQLCHAR*)sql.data(), sql.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return vector<vector<string>>();
	}
	//获取列的数目
	SQLSMALLINT colCount = 0;
	rcode = SQLNumResultCols(stmt, &colCount);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return vector<vector<string>>();
	}
	SQLLEN colLen = 0;
	SQLSMALLINT buf_len = 0;
	SQLLEN colType = 0;

	vector<vector<string>> v;

	while (true)
	{
		char sz_buf[256];
		//char pszBuf[1024];
		SQLLEN buflen;
		vector<string> t;
		if (SQLFetch(stmt) == SQL_NO_DATA)
		{
			break;
		}
		for (int i = 1; i <= colCount; i++)
		{
			string Data;
			//SQLColAttribute(stmt, i, SQL_DESC_NAME, sz_buf, 256, &buf_len, 0);
			//SQLColAttribute(stmt, i, SQL_DESC_TYPE, 0, 0, 0, &colType);
			SQLColAttribute(stmt, i, SQL_DESC_LENGTH, NULL, 0, 0, &colLen);
			char *pszBuf = new char[colLen + 1];
			pszBuf[0] = '\0';
			SQLGetData(stmt, i, SQL_C_CHAR, pszBuf, colLen + 1, &buflen);
			Data = pszBuf;
			t.push_back(Data);
		}
		v.push_back(t);
	}
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return v;
}

int Database::upData(string sql)
{
	// *) 分配语句句柄
	SQLHSTMT stmt;
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
	if (sql.empty())
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return -1;
	}
	rcode = SQLExecDirect(stmt, (SQLCHAR*)sql.data(), sql.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return -1;
	}
	SQLLEN rowCount = 0;
	rcode = SQLRowCount(stmt, &rowCount);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return -1;
	}
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return rowCount;
}

Database::~Database()
{
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
