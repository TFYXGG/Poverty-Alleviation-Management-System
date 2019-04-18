#include "Database.h"
#include <sstream>

Database::Database(string serverName, string userName, string passWorld):henv(NULL), hdbc(NULL)
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

vector<vector<string>> Database::query(string sql, vector<DataType> type)
{
	//获取字段数量
	int len = type.size();
	auto p = new void*[len];
	//申请空间
	for (int i = 0; i < len; i++)
	{
		switch (type[i])
		{
		case DataType::TYPECHAR:
		case DataType::TYPEINT_8:p[i] = new char; break;
		case DataType::TYPEINT_16:p[i] = new short; break;
		case DataType::TYPEINT_32:p[i] = new int; break;
		case DataType::TYPEINT_64:p[i] = new long long; break;
		case DataType::TYPESTRING:p[i] = new char[128]; break;		//字符串长度设置为128
		case DataType::TYPEFLOAT:p[i] = new float; break;
		case DataType::TYPEDOUBLE:p[i] = new double; break;
		case DataType::TYPETIMESTAMP:p[i] = new TIMESTAMP_STRUCT; break;
		}
	}
	// *) 创建SQL语句句柄
	SQLHSTMT stmt = NULL;
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) 执行具体的sql
	rcode = SQLExecDirect(stmt, (SQLCHAR*)sql.data(), sql.length());
	assert(!(rcode != SQL_SUCCESS && rcode != SQL_SUCCESS_WITH_INFO));
	// *) 绑定数据项
	SQLLEN res = SQL_NTS;
	for (int i = 0; i < len; i++)
	{
		switch (type[i])
		{
		case DataType::TYPECHAR:
		case DataType::TYPEINT_8:SQLBindCol(stmt, i + 1, SQL_C_CHAR, p[i], sizeof(char), &res);break;
		case DataType::TYPEINT_16:SQLBindCol(stmt, i + 1, SQL_C_SHORT, p[i], sizeof(short), &res);break;
		case DataType::TYPEINT_32:SQLBindCol(stmt, i + 1, SQL_C_SLONG, p[i], sizeof(int), &res);break;
		case DataType::TYPEINT_64:SQLBindCol(stmt, i + 1, SQL_C_SBIGINT, p[i], sizeof(long long), &res); break;
		case DataType::TYPESTRING:SQLBindCol(stmt, i + 1, SQL_C_CHAR, p[i], 128 * sizeof(char), &res); break;
		case DataType::TYPEFLOAT:SQLBindCol(stmt, i + 1, SQL_C_FLOAT, p[i], sizeof(float), &res); break;
		case DataType::TYPEDOUBLE:SQLBindCol(stmt, i + 1, SQL_C_DOUBLE, p[i], sizeof(double), &res); break;
		case DataType::TYPETIMESTAMP:SQLBindCol(stmt, i + 1, SQL_TYPE_TIMESTAMP, p[i], sizeof(SQL_TIMESTAMP_STRUCT), &res); break;
		}
	}
	// *)获取数据项
	vector<vector<string>> result;
	while ((rcode = SQLFetch(stmt)) != SQL_NO_DATA_FOUND)
	{
		if (rcode == SQL_ERROR)
		{
			printf("sql error!\n");
		}
		else
		{
			//string str;
			vector<string> t;
			for (int i = 0; i < len; i++)
			{
				stringstream ss;
				switch (type[i])
				{
				case DataType::TYPECHAR:ss << *(char*)p[i]; break;
				case DataType::TYPEINT_8:ss << dec << *(char*)p[i]; break;
				case DataType::TYPEINT_16:ss << *(short*)p[i]; break;
				case DataType::TYPEINT_32:ss << *(int*)p[i]; break;
				case DataType::TYPEINT_64:ss << *(long long*)p[i]; break;
				case DataType::TYPESTRING:ss << (char*)p[i]; break;
				case DataType::TYPEFLOAT:ss << *(float*)p[i]; break;
				case DataType::TYPEDOUBLE:ss << *(double*)p[i]; break;
				case DataType::TYPETIMESTAMP:ss << ((SQL_TIMESTAMP_STRUCT*)p[i])->year<<"-"<< ((SQL_TIMESTAMP_STRUCT*)p[i])->month<< ((SQL_TIMESTAMP_STRUCT*)p[i])->day; break;
				}
				t.push_back(ss.str());
			}
			result.push_back(t);
		}
	}
	//释放空间
	for (int i = 0; i < len; i++)
	{
		switch (type[i])
		{
		case DataType::TYPECHAR:
		case DataType::TYPEINT_8:delete (char *)p[i]; break;
		case DataType::TYPEINT_16:delete (short *)p[i]; break;
		case DataType::TYPEINT_32:delete (int *)p[i]; break;
		case DataType::TYPEINT_64:delete (long long *)p[i]; break;
		case DataType::TYPESTRING:delete[] (char *)p[i]; break;
		case DataType::TYPEFLOAT:delete (float *)p[i]; break;
		case DataType::TYPEDOUBLE:delete (double *)p[i]; break;
		case DataType::TYPETIMESTAMP:delete (SQL_TIMESTAMP_STRUCT*)p[i]; break;
		}
	}
	delete[] p;
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return result;
}

bool Database::upData(string sql)
{
	SQLHSTMT stmt;
	SQLRETURN rcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &stmt);
	if (!(rcode == SQL_SUCCESS || rcode == SQL_SUCCESS_WITH_INFO))
		return false;
	rcode = SQLExecDirect(stmt, (SQLCHAR*)sql.data(), sql.length());
	if (!(rcode== SQL_SUCCESS|| rcode == SQL_SUCCESS_WITH_INFO))
		return false;
	SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	return true;
}

Database::~Database()
{
	//SQLFreeHandle(SQL_HANDLE_STMT, stmt);
	SQLDisconnect(hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}
