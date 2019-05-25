#pragma once
#include <string>
#include <vector>

using namespace std;

class IdataBase
{
public:
	//执行sql语句并返回执行结果
	virtual vector<vector<string>> query(string const &sql) = 0;
	//执行更新类sql语句，失败返回-1 成功返回影响的行数
	virtual int upData(string const &sql) = 0;
	//释放与数据库连接，清理堆上内存
	virtual ~IdataBase() {};
};