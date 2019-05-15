#include "json.h"
#include <sstream>
#include <algorithm>


namespace Json
{
	value * pGetValue(const std::string &str, int & index)
	{
		switch (str.at(index))
		{
		case '{':return new object(str,index);
		case '[':return new array(str, index);
		case '"':return new strVal(str, index);
		default: return new numVal(str, index);
		}
	}
	bool ptrCmp::operator()(const strVal * s1, strVal * s2) const
	{
		return s1->str < s2->str;
	}
}

Json::strVal::strVal(const std::string &str, int & index):str()
{
	if (str.at(index) != '"')
		throw jsonException("strVal 初始化时找不到起始位置");
	index++;
	std::stringstream strstream;
	while (str.at(index) != '"')
	{
		try
		{
			strstream << str.at(index++);
		}
		catch (std::out_of_range e)
		{
			throw jsonException("strVal 初始化时无法找到结束标志");
		}
	}
	index++;
	this->str = strstream.str();
}

Json::strVal::strVal(const std::string & str):str(str)
{
}

std::string Json::strVal::toJsonString()
{
	return '"' + str + '"';
}

std::string Json::strVal::getCppString()
{
	return str;
}

Json::numVal::numVal(const std::string & str, int & index)
{
	std::stringstream strstream;
	try
	{
		while (true)
		{
			if (str.at(index) != ',' && str.at(index) != '}' && str.at(index) != ']')
				strstream << str.at(index++);
			else
				break;
		}
	}
	catch (std::out_of_range e)
	{
		throw jsonException("numVal 构建时无法找到结束标志");
	}
	this->str = strstream.str();
}

std::string Json::numVal::toJsonString()
{
	return str;
}

Json::object::object(const std::string & str, int & index)
{
	if (str.at(index) != '{')
		throw jsonException("object 初始化时找不到起始位置");
	index++;

	while(true)
	{ 
		strVal *key = new strVal(str, index);
		if (str.at(index++) != ':')
			throw jsonException("object 初始化时找不到符号\":\"");
		value *val = pGetValue(str, index);
		this->insert(std::pair<strVal *, value *>(key, val));
		if (str.at(index++) == '}')
			return;
	}
}

std::string Json::object::toJsonString()
{
	std::stringstream strstream;
	strstream << '{';
	for (auto it = this->begin(); it != this->end(); it++)
	{
		strstream << it->first->toJsonString() << ':' << it->second->toJsonString() << ',';
	}
	std::string str = strstream.str();
	str[str.length() - 1] = '}';
	return str;
}

Json::value * Json::object::at(std::string key)
{
	strVal sv(key);
	return this->map::at(&sv);
}

Json::object::~object()
{
	for (auto it = this->begin(); it != this->end(); it++)
	{
		delete it->first;
		delete it->second;
	}
}

Json::array::array(const std::string & str, int & index)
{
	if (str.at(index++) != '[')
		throw jsonException("array 构建时未找到起始位置");
	try
	{
		while (true)
		{
			auto p = pGetValue(str, index);
			this->push_back(p);
			if (str.at(index++) == ']')
				break;
		}
	}
	catch (std::out_of_range)
	{
		throw jsonException("array 构建时未找到结束标志");
	}
}

std::string Json::array::toJsonString()
{
	std::stringstream strstream;
	strstream << '[';
	for (auto it = this->begin(); it < this->end(); it++)
	{
		strstream << (*it)->toJsonString() << ',';
	}
	std::string str = strstream.str();
	str[str.length() - 1] = ']';
	return str;
}

Json::array::~array()
{
	for (auto it = this->begin(); it < this->end(); it++)
	{
		delete *it;
	}
}

Json::json::json(const std::string &jsonText):obj(nullptr)
{
	int index = 0;
	std::string str(jsonText);
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	obj = new object(str, index);
}

Json::object * Json::json::getRoot()
{
	return obj;
}

std::string Json::json::toJsonString()
{
	return obj->toJsonString();
}

Json::jsonException::jsonException(char const * const _Message):exception(_Message){}
