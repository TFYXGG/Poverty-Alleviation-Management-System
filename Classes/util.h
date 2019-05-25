#pragma once


#include <vector>
#include <string>
#include <map>

//�ָ��ַ��� ��һ������Ϊ���ָ��ַ��� �ڶ�������Ϊ�ָ��ʶ��
std::vector<std::string> split(std::string str, std::string const &pattern);

//��һ���ַ����ָ�洢��map��
//��һ������Ϊ���ָ��ַ���
//�ڶ�������Ϊ��ֵ�Լ�ָ���
//����������Ϊ����ֵ��ָ��
std::map<std::string, std::string> strToMap(std::string const &str, std::string const&, std::string const&);

//�����ִ�Сд�Ƚ� ��ȷ���true
bool compareNoCase(std::string const &str1, std::string const &str2);

//UTF-8���루%�����ת���ַ�����
std::string utf_82String(std::string str);

//���ַ�תUTF-8���루%�����
std::string string2Utf_8(std::string const &str);

//��ԭ���ĵ������ַ�
std::string reductionChinese(std::string const &str);

//�����Ľ��б���
std::string codingChinese(std::string const &str);

//UTF-8��GB2312��ת��
std::string U2G(std::string const &utf8);

//GB2312��UTF-8��ת��
std::string G2U(std::string const &gb2312);


//תjson�ļ�
//std::string arr2json(std::vector<std::string> name, std::vector<bool> type, std::vector<std::string> data);