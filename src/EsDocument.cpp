#include "EsDocument.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;
using namespace es;

EsDocument::EsDocument(std::string host, int port, int timeout)
{
	_host = host;
	_port = port;
	_timeout = timeout;
}

EsDocument::~EsDocument()
{}


Response EsDocument::deleteById(std::string index, std::string docId)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s/%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY, docId.c_str());

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_DELETE_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		fprintf(stderr, "%s\n", responseBody.c_str());
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}
	
	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}

Response EsDocument::deleteByQuery(std::string index, std::string query)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s", _host.c_str(), _port, index.c_str(), DOCUMENT_DELETE_BY_QUERY);
	httpUtil.setRequestBody(query);

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_POST_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}

Response EsDocument::create(std::string index, std::string query, std::string docId)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	std::string method;
	if ( docId.size() == 0 )
	{
		sprintf(requestUrl, "%s:%d/%s%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY);
		method = HTTP_POST_METHOD;
	}
	else
	{
		sprintf(requestUrl, "%s:%d/%s%s/%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY, docId.c_str());
		method = HTTP_PUT_METHOD;
	}

	httpUtil.setRequestBody(query);

	if ( false == httpUtil.sendHttpRequest(requestUrl, method.c_str(), headers,
				_timeout, responseBody, responseCode) )
	{
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}

Response EsDocument::update(std::string index, std::string query, std::string docId)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s/%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY, docId.c_str());
	httpUtil.setRequestBody(query);

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_POST_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}

Response EsDocument::get(DocResult& docResult, std::string index, std::string docId)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s/%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY, docId.c_str());

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_GET_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}
	
	// responseBody parsing
	Json::CharReaderBuilder builder;
	Json::CharReader * reader = builder.newCharReader();

	Json::Value root;
	std::string err;
	bool bParseResult = reader->parse(responseBody.c_str(), responseBody.c_str() + responseBody.size(), &root, &err);
	if ( !reader )
		delete reader;

	if ( false == bParseResult )
	{
		fprintf(stderr, "json parse failed \n");
		response.errorType = JSON_PARSING_ERROR_TYPE;
		response.errorMessage = JSON_PARSING_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}

	Json::StreamWriterBuilder writeBuilder;
	writeBuilder["indentation"] = "";

	std::string id = root["_id"].asString();
	int version = root["_version"].asInt();
	std::string source = Json::writeString(writeBuilder, root["_source"]);
	
	docResult.id = id;
	docResult.version = version;
	docResult.source = source;

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}

Response EsDocument::exists(std::string index, std::string docId)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s/%s", _host.c_str(), _port, index.c_str(), DOCUMENT_QUERY, docId.c_str());

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_HEAD_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		fprintf(stderr, "%s\n", responseBody.c_str());
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}
	
	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	return response;
}
