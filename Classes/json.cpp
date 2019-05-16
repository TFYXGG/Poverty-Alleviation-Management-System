#include "json.h"
#include <sstream>
#include <algorithm>
#include "util.h"


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
		throw jsonException("strVal ��ʼ��ʱ�Ҳ�����ʼλ��");
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
			throw jsonException("strVal ��ʼ��ʱ�޷��ҵ�������־");
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

Json::numVal::numVal(const std::string & str):str(str)
{
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
		throw jsonException("numVal ����ʱ�޷��ҵ�������־");
	}
	this->str = strstream.str();
}

std::string Json::numVal::toJsonString()
{
	return str;
}

float Json::numVal::getFloat()
{
	return atof(str.data());
}

double Json::numVal::getDouble()
{
	double d;
	return getCppValue(d);
}

int Json::numVal::getInt()
{
	return atoi(str.data());
}

std::string Json::numVal::getString()
{
	return str;
}

char Json::numVal::getChar()
{
	return str.at(0);
}

bool Json::numVal::getBool()
{
	if (compareNoCase("true", str))
		return true;
	return false;
}

Json::object::object()
{}

Json::object::object(const std::string & str, int & index)
{
	if (str.at(index) != '{')
		throw jsonException("object ��ʼ��ʱ�Ҳ�����ʼλ��");
	index++;

	while(true)
	{ 
		strVal *key = new strVal(str, index);
		if (str.at(index++) != ':')
			throw jsonException("object ��ʼ��ʱ�Ҳ�������\":\"");
		value *val = pGetValue(str, index);
		this->insert(std::pair<strVal *, value *>(key, val));
		if (str.at(index++) == '}')
			return;
	}
}

void Json::object::add(std::string key, value *val)
{
	this->insert(map::value_type(new strVal(key), val));
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

Json::array::array()
{
}

Json::array::array(const std::string & str, int & index)
{
	if (str.at(index++) != '[')
		throw jsonException("array ����ʱδ�ҵ���ʼλ��");
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
		throw jsonException("array ����ʱδ�ҵ�������־");
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

Json::json::json()
{
}

Json::json::json(const std::string &jsonText, std::string encoding):obj(nullptr)
{
	int index = 0;
	std::string str;
	if (encoding == "GBK")
		str = jsonText;
	else if (encoding == "UTF8" || encoding == "UTF-8")
		str = U2G(jsonText);
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\t'), str.end());
	obj = new object(str, index);
}

Json::json::json(object * obj):obj(obj)
{
}

Json::object * Json::json::getRoot()
{
	return obj;
}

void Json::json::setRoot(object * Root)
{
	obj = Root;
}

std::string Json::json::toJsonString()
{
	return obj->toJsonString();
}

std::string Json::json::toJsonFile(std::string encoding)
{
	if (encoding == "GBK")
		return "\xef\xbb\xbf" + G2U(toJsonString());
	if (encoding == "UTF8" || encoding == "UTF-8")
		return "\xef\xbb\xbf" + toJsonString();
}

Json::jsonException::jsonException(char const * const _Message):errorMessage(_Message){}

const char * Json::jsonException::what() const throw()
{
	return errorMessage.data();
}
