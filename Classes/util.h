#pragma once


#include <vector>
#include <string>
#include <map>

//分割字符串 第一个参数为待分割字符串 第二个参数为分割标识符
std::vector<std::string> split(std::string str, std::string const &pattern);

//将一个字符串分割并存储与map中
//第一个参数为待分割字符串
//第二个参数为键值对间分隔符
//第三个参数为键与值间分割符
std::map<std::string, std::string> strToMap(std::string const &str, std::string const&, std::string const&);

//不区分大小写比较 相等返回true
bool compareNoCase(std::string const &str1, std::string const &str2);

//UTF-8编码（%间隔）转单字符编码
std::string utf_82String(std::string str);

//单字符转UTF-8编码（%间隔）
std::string string2Utf_8(std::string const &str);

//还原中文等特殊字符
std::string reductionChinese(std::string const &str);

//对中文进行编码
std::string codingChinese(std::string const &str);

//UTF-8到GB2312的转换
std::string U2G(std::string const &utf8);

//GB2312到UTF-8的转换
std::string G2U(std::string const &gb2312);


//转json文件
//std::string arr2json(std::vector<std::string> name, std::vector<bool> type, std::vector<std::string> data);