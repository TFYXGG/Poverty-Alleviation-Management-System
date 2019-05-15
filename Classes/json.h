

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <exception>

namespace Json
{
	class jsonException :public std::exception
	{	
	public:
		jsonException(char const* const _Message);
	};

	class value
	{
	public:
		virtual std::string toJsonString() = 0;
		virtual ~value() {};
	};

	//字符串类型
	class strVal :public value
	{
	public:
		strVal(const std::string &str, int &index);
		strVal(const std::string &str);
		virtual std::string toJsonString() override;
		std::string getCppString();
		friend class ptrCmp;
	private:
		std::string str;
	};

	//数值类型
	class numVal :public value
	{
	public:
		numVal(const std::string &str, int &index);
		virtual std::string toJsonString() override;
		template<typename T>
		T& getCppValue(T &t)
		{
			std::stringstream strstream(str);
			strstream >> t;
			return t;
		}
	private:
		std::string str;
	};

	class ptrCmp
	{
	public:
		bool operator()(const strVal* s1, strVal* s2) const;
	};

	//对象
	class object :public value,public std::map<strVal*,value*,ptrCmp>
	{
	public:
		object(const std::string &str, int &index);
		virtual std::string toJsonString() override;
		value* at(std::string key);
		virtual ~object() override;
	};
	//数组

	class array :public value, public std::vector<value*>
	{
	public:
		array(const std::string &str, int &index);
		virtual std::string toJsonString() override;
		virtual ~array() override;
	};

	class json
	{
	public:
		json(const std::string &jsonText);
		std::string toJsonString();
		object * getRoot();
	private:
		object *obj;
	};
}