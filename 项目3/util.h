#pragma once

#include <vector>
#include <string>
#include <map>

//�ָ��ַ��� ��һ������Ϊ���ָ��ַ��� �ڶ�������Ϊ�ָ��ʶ��
std::vector<std::string> split(std::string str, std::string pattern);

//��һ���ַ����ָ�洢��map��
//��һ������Ϊ���ָ��ַ���
//�ڶ�������Ϊ��ֵ�Լ�ָ���
//����������Ϊ����ֵ��ָ��
std::map<std::string, std::string> strToMap(std::string str, std::string, std::string);

//�����ִ�Сд�Ƚ� ��ȷ���true
bool compareNoCase(std::string str1, std::string str2);

//UTF-8���루%�����ת���ַ�����
std::string utf_82String(std::string str);

//���ַ�תUTF-8���루%�����
std::string string2Utf_8(std::string str);