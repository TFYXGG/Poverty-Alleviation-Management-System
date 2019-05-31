#pragma once

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
#ifdef LINUX
		const char* what() const noexcept;
	private:
		char errorMessage[256];
#endif // LINUX
	};

	class value
	{
	public:
		virtual std::string toJsonString() = 0;
		virtual ~value() = default;
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
		numVal(const std::string &str);
		numVal(const std::string &str, int &index);
		virtual std::string toJsonString() override;
		float getFloat();
		double getDouble();
		int getInt();
		std::string getString();
		char getChar();
		bool getBool();
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
		object();
		object(const std::string &str, int &index);
		void add(std::string const &key,value *val);
		virtual std::string toJsonString() override;
		value* at(std::string const &key);
		virtual ~object() override;
	};

	//数组
	class array :public value, public std::vector<value*>
	{
	public:
		array();
		array(const std::string &str, int &index);
		virtual std::string toJsonString() override;
		virtual ~array() override;
	};

	class json
	{
	public:
		json();
		json(const std::string &jsonText,std::string const &encoding = "GBK");
		json(object *obj);
		std::string toJsonString();
		std::string toJsonFile(std::string const &encoding = "UTF8");
		object * getRoot();
		void setRoot(object *Root);
		~json();
	private:
		object *obj;
	};
}