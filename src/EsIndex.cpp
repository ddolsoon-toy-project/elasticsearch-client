#include "EsIndex.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;
using namespace es;

EsIndex::EsIndex(std::string host, int port, int timeout)
{
	_host = host;
	_port = port;
	_timeout = timeout;
}

EsIndex::~EsIndex()
{}


Response EsIndex::deleteIndex(std::string index)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s", _host.c_str(), _port, index.c_str());

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_DELETE_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		fprintf(stderr, "%s\n", responseBody.c_str());
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		response.body = responseBody;
		return response;
	}
	
	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;
	response.body = responseBody;
	
	return response;
}

Response EsIndex::createIndex(std::string index, std::string mappings)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s", _host.c_str(), _port, index.c_str());
	httpUtil.setRequestBody(mappings);

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_PUT_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		fprintf(stderr, "%s\n", responseBody.c_str());
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		response.body = responseBody;
		return response;
	}
	
	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;
	response.body = responseBody;
	
	return response;
}

Response EsIndex::existsIndex(std::string index)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s", _host.c_str(), _port, index.c_str());

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_HEAD_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		fprintf(stderr, "%s\n", responseBody.c_str());
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		response.body = responseBody;
		return response;
	}
	
	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;
	response.body = responseBody;


	return response;
}

