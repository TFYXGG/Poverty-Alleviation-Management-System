#pragma once

#include <vector>
#include <string>
#include <map>

//分割字符串 第一个参数为待分割字符串 第二个参数为分割标识符
std::vector<std::string> split(std::string str, std::string pattern);

//将一个字符串分割并存储与map中
//第一个参数为待分割字符串
//第二个参数为键值对间分隔符
//第三个参数为键与值间分割符
std::map<std::string, std::string> strToMap(std::string str, std::string, std::string);

//不区分大小写比较
bool compareNoCase(std::string str1, std::string str2);