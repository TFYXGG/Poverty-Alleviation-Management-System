
#include <string>
#include <vector>

using namespace std;

enum DataType
{
	TYPECHAR = 1,		//�ַ��� char
	TYPESTRING,			//�ַ��� char*
	TYPEINT_8,			//8λ���� char
	TYPEINT_16,			//16λ���� short
	TYPEINT_32,			//32λ���� int
	TYPEINT_64,			//64λ���� long long
	TYPEFLOAT,			//�����ȸ����� float
	TYPEDOUBLE,			//˫���ȸ����� double
	TYPETIMESTAMP		//������ SQL_TIMESTAMP_STRUCT �ṹ��
};

class IdataBase
{
public:
	//���캯�������� IP �˿� ��½�� ����
	//IdataBase(string host,string port,string name,string passWorld);
	//ִ��sql��䲢����ִ�н�� type�������ͼ��� ȡֵΪDataTypeö�ٵ�ֵ ����ֻ����������
	virtual vector<vector<string>> query(string sql,vector<DataType> type) = 0;
	//ִ�и�����sql��䣬�ɹ�����true ʧ�ܷ���false
	virtual bool upData(string sql) = 0;
	//�ͷ������ݿ����ӣ���������ڴ�
	virtual ~IdataBase() {};
};