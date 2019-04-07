#include "util.h"
#include <algorithm>

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;

	str += pattern;//扩展字符串以方便操作
	auto size = str.size();

	for (unsigned int i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

std::map<std::string, std::string> strToMap(std::string str, std::string pairPattern, std::string KVpattern)
{
	std::map<std::string, std::string> result;
	std::vector<std::string> element = split(str, pairPattern);
	for (std::vector<std::string>::iterator begin = element.begin(); begin != element.end(); begin++)
	{
		int n = begin->find(KVpattern);
		if (n == -1)
		{
			return result;
		}
		result.insert(std::pair<std::string, std::string>(std::string((*begin), 0, n), std::string((*begin), n + 1)));
	}
	return result;
}

std::string strToLower(const std::string str)
{
	std::string strTmp = str;
	transform(strTmp.begin(), strTmp.end(), strTmp.begin(), towupper);
	return strTmp;
}

bool compareNoCase(std::string str1, std::string str2)
{
	std::string strA = strToLower(str1);
	std::string strB = strToLower(str2);
	return (strA == strB);
}
