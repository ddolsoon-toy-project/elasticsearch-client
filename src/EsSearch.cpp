#include "EsSearch.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;

EsSearch::EsSearch(std::string host, int port, int timeout)
{
	_host = host;
	_port = port;
	_timeout = timeout;
}

EsSearch::~EsSearch()
{}

Response EsSearch::search(SearchResult& searchResult, std::string index, std::string query)
{
	Response response;

	// HTTP Request
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d/%s%s", _host.c_str(), _port, index.c_str(), SEARCH_QUERY);
	httpUtil.setRequestBody(query);

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
	if ( false == reader->parse(responseBody.c_str(), responseBody.c_str() + responseBody.size(), &root, &err) )
	{
		delete reader;
		fprintf(stderr, "json parse failed \n");
		response.errorType = JSON_PARSING_ERROR_TYPE;
		response.errorMessage = JSON_PARSING_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		return response;
	}
	delete reader;

	int totalCount = root["hits"]["total"]["value"].asInt();
	double maxScore = root["hits"]["max_score"].asDouble();
	std::string hits = root["hits"]["hits"].toStyledString();

	searchResult.totalCount = totalCount;
	searchResult.maxScore = maxScore;
	searchResult.hits = hits;

	fprintf(stderr, "totalCount : %d , maxScore : %lf \n", totalCount, maxScore);
	fprintf(stderr, "hits : %s \n", hits.c_str());
	fprintf(stderr, "query Result \n\n %s \n\n", responseBody.c_str());

	return response;
}

Response EsSearch::search(SearchResult& searchResult,SearchQuery searchQuery)
{
	Response response;

	
	return response;
}
