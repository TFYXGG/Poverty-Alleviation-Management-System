#include "util.h"
#include <algorithm>
#include <sstream>
#include <cstring>
#ifdef LINUX
#include <iconv.h>
#endif // LINUX

#ifdef WINDOWS
#include <Windows.h>
#include <codecvt>
#endif // WINDOWS



std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;

	str += pattern;//À©Õ¹×Ö·û´®ÒÔ·½±ã²Ù×÷
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

#ifdef WINDOWS

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

std::string UnicodeToUtf8(const std::wstring &strUnicode)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0)
	{
		return "";
	}

	char *pRes = new char[len];
	if (pRes == NULL)
	{
		return "";
	}

	WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), -1, pRes, len, NULL, NULL);
	pRes[len - 1] = '\0';
	std::string result = pRes;
	delete[] pRes;

	return result;
}

std::string string2Utf_8(std::string str)
{
	setlocale(LC_ALL, "chs");
	wchar_t pwcs[256];
	mbstowcs(pwcs, str.c_str(), str.length());
	std::string s = UnicodeToUtf8(pwcs);
	unsigned char uc[256] = { 0 };
	memcpy(uc, s.data(), s.length());
	std::stringstream ss;
	for (int i = 0; i < s.length(); i++)
	{
		ss << "%" << std::hex << (int)uc[i];
	}
	return ss.str();
}
#endif // WINDOWS

#ifdef LINUX

//UTF-8±àÂë£¨%¼ä¸ô£©×ªµ¥×Ö·û±àÂë
std::string utf_82String(std::string str)
{
	auto stream = iconv_open("GBK", "UTF-8");

	str.erase(std::remove(str.begin(), str.end(), '%'), str.end());
	int tempLen = str.length();
	int UTF8Len = tempLen / 2;
	char *UTF_8Code = new char[UTF8Len + 1]{ 0 };
	for (int i = 0; i < UTF8Len * 2;)
	{
		char convert[3] = { 0 };
		convert[0] = str[i++];
		convert[1] = str[i++];
		int tempint = strtol(convert, nullptr, 16);
		UTF_8Code[i / 2 - 1] = tempint;
	}

	char inbuf[1024];
	memcpy(inbuf, UTF_8Code, UTF8Len + 1);
	delete[] UTF_8Code;
	size_t srclen = strlen(inbuf);
	size_t outlen = 1024;
	char outbuf[outlen];
	memset(outbuf, 0, outlen);

	char *srcstart = inbuf;
	char *tempoutbuf = outbuf;

	size_t ret = iconv(stream, &srcstart, &srclen, &tempoutbuf, &outlen);
	if (ret == -1)
	{
		perror("iconv");
	}

	iconv_close(stream);
	return std::string(outbuf);
}

//µ¥×Ö·û×ªUTF-8±àÂë£¨%¼ä¸ô£©
std::string string2Utf_8(std::string str)
{
	char inbuf[1024] = { 0 };
	char outbuf[1024] = { 0 };
	strcpy(inbuf, str.data());
	char *srcstart = inbuf;
	char *tempoutbuf = outbuf;
	auto stream = iconv_open("UTF8", "GBK");
	size_t srclen = str.length();
	size_t outlen = 1024;
	size_t ret = iconv(stream, &srcstart, &srclen, &tempoutbuf, &outlen);
	if (ret == -1)
	{
		perror("iconv");
	}
	unsigned char buf[1024] = { 0 };
	memcpy(buf, outbuf, ret);
	std::stringstream ss;
	for (int i = 0; i < ret; i++)
	{
		ss << "%" << std::hex << buf[i];
	}
	iconv_close(stream);
	return ss.str();
}

#endif // LINUX


std::string reductionChinese(std::string str)
{
	std::stringstream ss;
	for (int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '%')
		{
			std::stringstream st;
			for (; i < str.length() - 2 && str.at(i) == '%'; i += 3)
			{
				st << str[i];
				st << str[i + 1];
				st << str[i + 2];
			}
			ss << utf_82String(st.str());
		}
		ss << str[i];
	}
	return ss.str();
}

std::string codingChinese(std::string str)
{
	std::stringstream ss;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] >= 0 && str[i] <= 127)
		{
			ss << str[i];
		}
		else
		{
			/*char buf[1024] = { 0 };
			int j = 0;*/
			std::stringstream st;
			for (; i < str.length() && !(str[i] >= 0 && str[i] <= 127); i++)
			{
				//buf[j++] = str[i];
				st << str[i];
			}
			ss << string2Utf_8(st.str());
			i--;
		}
	}
	return ss.str();
}


std::string U2G(std::string utf8)
{
#ifdef WINDOWS
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8.data(), -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	std::string s = str;
	delete[] str;
	return s;
#endif // WINDOWS
#ifdef LINUX
	auto stream = iconv_open("GBK", "UTF8");
	char *input = new char[utf8.length() + 1]{ 0 };
	char *output = new char[utf8.length() * 2]{ 0 };
	strcpy(input, utf8.data());
	char *srcstart = input;
	size_t srclen = utf8.length();
	char *tempoutbuf = output;
	size_t outlen = utf8.length() * 2;
	size_t ret = iconv(stream, &srcstart, &srclen, &tempoutbuf, &outlen);
	if (ret == -1)
		return "";
	std::string str(output);
	delete[] output;
	delete[] input;
	return str;
#endif // LINUX
}

std::string G2U(std::string gb2312)
{
#ifdef WINDOWS
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312.data(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312.data(), -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	std::string s = str;
	delete[] str;
	return s;
#endif // WINDOWS
#ifdef LINUX
	auto stream = iconv_open("UTF8", "GBK");
	char *input = new char[gb2312.length() + 1]{ 0 };
	char *output = new char[gb2312.length() + 1]{ 0 };
	strcpy(input, gb2312.data());
	char *srcstart = input;
	size_t srclen = gb2312.length();
	char *tempoutbuf = output;
	size_t outlen = gb2312.length();
	size_t ret = iconv(stream, &srcstart, &srclen, &tempoutbuf, &outlen);
	if (ret == -1)
		return "";
	std::string str(output);
	delete[] output;
	delete[] input;
	return str;
#endif // LINUX
}

