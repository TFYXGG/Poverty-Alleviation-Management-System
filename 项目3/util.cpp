#include "util.h"
#include <algorithm>
#include <Windows.h>
#include <codecvt>

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

std::string utf_82String(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '%'), str.end());
	int tempLen = str.length();
	int UTF8Len = tempLen / 2;
	char * const UTF_8Code = new char[UTF8Len + 1]{ 0 };
	for (int i = 0; i < UTF8Len * 2;)
	{
		char convert[3] = { 0 };
		convert[0] = str[i++];
		convert[1] = str[i++];
		int tempint = strtol(convert, nullptr, 16);
		UTF_8Code[i / 2 - 1] = tempint;
	}
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::wstring wString = conv.from_bytes(UTF_8Code);
	delete[] UTF_8Code;
	char buf[256] = { 0 };
	setlocale(LC_ALL, "chs");
	wcstombs(buf, wString.c_str(), 256);
	setlocale(LC_ALL, "c");
	return buf;
}

std::string string2Utf_8(std::string str)
{
	//未实现
	return "";
}

