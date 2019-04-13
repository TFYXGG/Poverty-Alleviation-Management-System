#include "httpMessage.h"

#include "util.h"
#include <fstream>
#include <string>
#include <sstream>



RequestMessage::RequestMessage(char * message, int len):entityBody(nullptr)
{
	//按换行符分割
	std::vector<std::string> vs = split(message, "\r\n");
	//起始行初始化
	//起始行分割
	std::vector<std::string> startLineVs = split(vs.at(0), " ");
	method = startLineVs.at(0);
	std::vector<std::string> URL = split(startLineVs.at(1), "?");
	requestURL = URL.at(0);
	try {
		requestParameter = strToMap(URL.at(1), "&", "=");
	}
	catch (std::out_of_range e)
	{
		//std::cout <<"初始化URL参数时发生异常:"<<e.what() << std::endl;
	}
	version = startLineVs.at(2);
	//消息头部初始化
	for (int i = 1; i < vs.size(); i++)
	{
		if (vs.at(i).length() <= 0)
			break;
		std::vector<std::string> headersVs = split(vs.at(i), ": ");
		headers.insert(std::pair<std::string, std::string>(headersVs.at(0), headersVs.at(1)));
	}
	//初始化消息体
	int bodyLen = 0;
	try {
		bodyLen = atoi(getHeaders("Content-Length").c_str());
		entityBody = new char[len];
		memcpy(entityBody, message + len - bodyLen, bodyLen);
	}catch(std::out_of_range e)
	{
		//std::cout << "初始化消息体时发生异常:" << e.what() << std::endl;
	}
}

const std::string & RequestMessage::getMethod() const
{
	return method;
}

const std::string & RequestMessage::getURL() const
{
	return requestURL;
}

const std::string & RequestMessage::getParameter(std::string key) const
{
	return requestParameter.at(key);
}

const std::map<std::string, std::string>& RequestMessage::getParameter() const
{
	return requestParameter;
}

const std::string & RequestMessage::getVersion() const
{
	return version;
}

const std::string & RequestMessage::getHeaders(const std::string key) const
{
	return headers.at(key);
}

const std::map<std::string, std::string>& RequestMessage::getHeaders() const
{
	return headers;
}

const char * RequestMessage::getBody() const
{
	return entityBody;
}

RequestMessage::~RequestMessage()
{
	if (entityBody != nullptr)
	{
		delete[] entityBody;
	}
}

std::string ResponseMessage::getStatusString(HTTPStatusCode hsc)
{
	switch (hsc)
	{
	case uninitialized:
		return "Uninitialized";
	case continue_code:
		return "Continue";
	case switching_protocols:
		return "Switching Protocols";
	case ok:
		return "OK";
	case created:
		return "Created";
	case accepted:
		return "Accepted";
	case non_authoritative_information:
		return "Non Authoritative Information";
	case no_content:
		return "No Content";
	case reset_content:
		return "Reset Content";
	case partial_content:
		return "Partial Content";
	case multiple_choices:
		return "Multiple Choices";
	case moved_permanently:
		return "Moved Permanently";
	case found:
		return "Found";
	case see_other:
		return "See Other";
	case not_modified:
		return "Not Modified";
	case use_proxy:
		return "Use Proxy";
	case temporary_redirect:
		return "Temporary Redirect";
	case bad_request:
		return "Bad Request";
	case unauthorized:
		return "Unauthorized";
	case payment_required:
		return "Payment Required";
	case forbidden:
		return "Forbidden";
	case not_found:
		return "Not Found";
	case method_not_allowed:
		return "Method Not Allowed";
	case not_acceptable:
		return "Not Acceptable";
	case proxy_authentication_required:
		return "Proxy Authentication Required";
	case request_timeout:
		return "Request Timeout";
	case conflict:
		return "Conflict";
	case gone:
		return "Gone";
	case length_required:
		return "Length Required";
	case precondition_failed:
		return "Precondition Failed";
	case request_entity_too_large:
		return "Request Entity Too Large";
	case request_uri_too_long:
		return "Request-URI Too Long";
	case unsupported_media_type:
		return "Unsupported Media Type";
	case request_range_not_satisfiable:
		return "Requested Range Not Satisfiable";
	case expectation_failed:
		return "Expectation Failed";
	case im_a_teapot:
		return "I'm a teapot";
	case upgrade_required:
		return "Upgrade Required";
	case precondition_required:
		return "Precondition Required";
	case too_many_requests:
		return "Too Many Requests";
	case request_header_fields_too_large:
		return "Request Header Fields Too Large";
	case internal_server_error:
		return "Internal Server Error";
	case not_implemented:
		return "Not Implemented";
	case bad_gateway:
		return "Bad Gateway";
	case service_unavailable:
		return "Service Unavailable";
	case gateway_timeout:
		return "Gateway Timeout";
	case http_version_not_supported:
		return "HTTP Version Not Supported";
	case not_extended:
		return "Not Extended";
	case network_authentication_required:
		return "Network Authentication Required";
	default:
		return "Unknown";
	}
}

ResponseMessage::ResponseMessage() :version(),status(),reasonPhrase(),headers(),entityBody(nullptr)
{
}

void ResponseMessage::setVersion(const std::string & version)
{
	this->version = version;
}

void ResponseMessage::setStatus(const std::string & status)
{
	this->status = status;
}

void ResponseMessage::setPhrase(const std::string & phrase)
{
	this->reasonPhrase = phrase;
}

void ResponseMessage::setHeaders(const std::string & key, const std::string & value)
{
	headers[key] = value;
}

void ResponseMessage::setHeaders(const std::map<std::string, std::string>& pair)
{
	headers = pair;
}

void ResponseMessage::setBody(const char * data, int size)
{
	if (entityBody!= nullptr)
	{
		delete[] entityBody;
	}
	entityBody = new char[size];
	memcpy(this->entityBody, data, size);
}

bool ResponseMessage::openFileSetBody(std::string fileName)
{
	int len = 0;
	//以二进制方式打开文件
	std::ifstream file(fileName,std::ios::binary);
	if (!file.is_open())
	{
		//std::cout << fileName << "打开失败" << std::endl;
		file.close();
		return false;
	}
	//获取文件大小
	file.seekg(0, std::ios::end);
	len = file.tellg();
	//重新分配空间
	if (entityBody != nullptr)
	{
		delete[] entityBody;
	}
	entityBody = new char[len] {0};
	//读取文件内容
	file.seekg(0, std::ios::beg);
	file.read(entityBody, len);
	setHeaders("Content-Length", std::to_string(len));
	file.close();
	return true;
}

ResponseMessage::~ResponseMessage()
{
	if (entityBody != nullptr)
		delete[] entityBody;
}

int ResponseMessage::getByte(char *&buf)
{
	std::string rs;
	std::ostringstream rss;
	rss << this->version << " " << this->status << " " << this->reasonPhrase << "\r\n";
	for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); it++)
	{
		rss << it->first << ": " << it->second << "\r\n";
	}
	rss << "\r\n";
	rs = rss.str();
	int bodyLen = 0;
	try {
		bodyLen = atoi(this->headers.at("Content-Length").data());
	}
	catch (std::out_of_range e)
	{
		//std::cout << "获取body内容时引发了异常:" << e.what() << std::endl;
	}
	int len = bodyLen + rs.size();
	if (buf != nullptr)
	{
		delete[] buf;
	}
	buf = new char[len];
	memcpy(buf, rs.c_str(), rs.size());
	memcpy(buf + rs.size(), entityBody, bodyLen);
	return len;
}
