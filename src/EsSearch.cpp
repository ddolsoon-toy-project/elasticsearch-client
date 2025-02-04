#include "EsSearch.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;
using namespace es;

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
		response.body = responseBody;
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
		response.body = responseBody;
		return response;
	}

	int totalCount = root["hits"]["total"]["value"].asInt();
	double maxScore = root["hits"]["max_score"].asDouble();

	Json::StreamWriterBuilder writeBuilder;
	writeBuilder["emitUTF8"] = true;

	std::string hits = Json::writeString(writeBuilder, root["hits"]["hits"]);
	std::string aggs = "";
	if ( root.isMember("aggregations") )
	{
		aggs = Json::writeString(writeBuilder, root["aggregations"]);
	}

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;

	searchResult.totalCount = totalCount;
	searchResult.maxScore = maxScore;
	searchResult.hits = hits;
	searchResult.aggs = aggs;

	fprintf(stderr, "totalCount : %d , maxScore : %lf \n", totalCount, maxScore);
	fprintf(stderr, "hits : %s \n", hits.c_str());
	// fprintf(stderr, "query Result \n\n %s \n\n", responseBody.c_str());

	return response;
}

Response EsSearch::search(SearchResult& searchResult,SearchQuery searchQuery)
{
	Response response;

	
	return response;
}
