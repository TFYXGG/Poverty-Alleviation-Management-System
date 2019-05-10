#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

class RequestMessage	//请求报文
{
public:
	//构造
	RequestMessage(char *message, int len);
	//获取请求方式
	const std::string& getMethod()const;
	//获取URL
	const std::string& getURL()const;
	//获取指定的URL参数
	const std::string& getParameter(std::string key) const/* throw(std::out_of_range)*/;
	//获取所有URL参数 以键值对方式返回
	const std::map<std::string, std::string>& getParameter()const;
	//获取协议版本
	const std::string& getVersion()const;
	//获取消息头部指定信息
	const std::string& getHeaders(const std::string key)const/* throw(std::out_of_range)*/;
	//获取消息头部所有信息
	const std::map<std::string, std::string>&getHeaders()const;
	//获取消息主体
	const char* getBody()const;
	//析构
	~RequestMessage();
private:
	std::string method;		//请求方式
	std::string requestURL;	//请求URL
	std::map<std::string, std::string> requestParameter;	//请求URL参数
	std::string version;	//报文所使用的HTTP版本
	std::map<std::string, std::string> headers;	//请求头部
	char *entityBody;		//请求实体
};

class ResponseMessage		//响应报文
{
public:
	//http响应状态码
	enum HTTPStatusCode
	{
		uninitialized = 0,

		continue_code = 100,
		switching_protocols = 101,

		ok = 200,
		created = 201,
		accepted = 202,
		non_authoritative_information = 203,
		no_content = 204,
		reset_content = 205,
		partial_content = 206,

		multiple_choices = 300,
		moved_permanently = 301,
		found = 302,
		see_other = 303,
		not_modified = 304,
		use_proxy = 305,
		temporary_redirect = 307,

		bad_request = 400,
		unauthorized = 401,
		payment_required = 402,
		forbidden = 403,
		not_found = 404,
		method_not_allowed = 405,
		not_acceptable = 406,
		proxy_authentication_required = 407,
		request_timeout = 408,
		conflict = 409,
		gone = 410,
		length_required = 411,
		precondition_failed = 412,
		request_entity_too_large = 413,
		request_uri_too_long = 414,
		unsupported_media_type = 415,
		request_range_not_satisfiable = 416,
		expectation_failed = 417,
		im_a_teapot = 418,
		upgrade_required = 426,
		precondition_required = 428,
		too_many_requests = 429,
		request_header_fields_too_large = 431,

		internal_server_error = 500,
		not_implemented = 501,
		bad_gateway = 502,
		service_unavailable = 503,
		gateway_timeout = 504,
		http_version_not_supported = 505,
		not_extended = 510,
		network_authentication_required = 511
	};
	//获取http响应状态字
	static std::string getStatusString(HTTPStatusCode hsc);
	//构造
	ResponseMessage();
	//设置版本
	void setVersion(const std::string& version);
	//设置状态码
	void setStatus(const std::string& status);
	//设置原因短语
	void setPhrase(const std::string& phrase);
	//设置（添加）一个头部键值对 重复添加（key重复）将覆盖（value）原数据
	void setHeaders(const std::string& key, const std::string& value);
	//用一个键值对集合（map）重设头部
	void setHeaders(const std::map<std::string, std::string>& pair);
	//设置消息主体 消息主体将重新分配空间（与传入参数不共享空间）
	void setBody(const char * data, int size);
	//打开文件设置消息体
	bool openFileSetBody(std::string fileName);
	//析构
	~ResponseMessage();
	//转换到字节
	int getByte(char *&buf);
private:
	std::string version;	//报文所使用的HTTP版本
	std::string status;		//状态码
	std::string reasonPhrase;	//原因短语
	std::map<std::string, std::string> headers;	//请求头部
	char *entityBody;		//请求实体
};