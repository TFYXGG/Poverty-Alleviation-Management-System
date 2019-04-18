
#include <string>
#include <vector>

using namespace std;

enum DataType
{
	TYPECHAR = 1,		//字符型 char
	TYPESTRING,			//字符串 char*
	TYPEINT_8,			//8位整型 char
	TYPEINT_16,			//16位整型 short
	TYPEINT_32,			//32位整型 int
	TYPEINT_64,			//64位整型 long long
	TYPEFLOAT,			//单精度浮点型 float
	TYPEDOUBLE,			//双精度浮点型 double
	TYPETIMESTAMP		//日期型 SQL_TIMESTAMP_STRUCT 结构体
};

class IdataBase
{
public:
	//构造函数，参数 IP 端口 登陆名 密码
	//IdataBase(string host,string port,string name,string passWorld);
	//执行sql语句并返回执行结果 type数据类型集合 取值为DataType枚举的值 日期只返回年月日
	virtual vector<vector<string>> query(string sql,vector<DataType> type) = 0;
	//执行更新类sql语句，成功返回true 失败返回false
	virtual bool upData(string sql) = 0;
	//释放与数据库连接，清理堆上内存
	virtual ~IdataBase() {};
};