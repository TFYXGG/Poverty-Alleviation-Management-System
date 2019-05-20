#include "Database.h"
#include <sstream>
#include <iostream>

bool Database::sign = false;
HENV Database::henv = nullptr;

Database::Database(string serverName, string userName, string passWorld) : hdbc(NULL)
{
	if (!sign)
	{
		Init();
		sign = true;
	}
	// *) �������Ӿ��
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) ��������Դ
	rcode = SQLConnect(hdbc, (SQLCHAR *)serverName.data(), serverName.length(),
		(SQLCHAR *)userName.data(), userName.length(), (SQLCHAR *)passWorld.data(), passWorld.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
}

vector<vector<string>> Database::query(string sql)
{
	// *) ���������
	SQLHSTMT stmt;
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return vector<vector<string>>();
	}
	//ִ�о����sql
	rcode = SQLExecDirect(stmt, (SQLCHAR*)sql.data(), sql.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
	{
		SQLFreeHandle(SQL_HANDLE_STMT, stmt);
		return vector<vector<string>>();
	}
	//��ȡ�е���Ŀ
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
			delete[] pszBuf;
		}
		v.push_back(t);
	}
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return v;
}

int Database::upData(string sql)
{
	// *) ���������
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
	//SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

void Database::Init()
{
	SQLRETURN rcode;
	// *) ���뻷�����
	rcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) ����ODBC�汾�Ļ�������
	rcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, SQL_IS_INTEGER);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
}