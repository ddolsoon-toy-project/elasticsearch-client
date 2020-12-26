#include "EsClient.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"


using namespace tinyxml2;

EsClient::EsClient(std::string host, int port, int timeout, std::string queryPath)
{
	_strHost = host;
	_nPort = port;
	_nTimeout = timeout;
	_pQueryMapper = new QueryMapper(queryPath);
	_pEsSearch = new EsSearch(host, port, timeout);
	_pEsIndex = new EsIndex(host, port, timeout);
	_pEsDocument = new EsDocument(host, port, timeout);

	// QueryMapper 초기화
	_pQueryMapper->initialize(queryPath);
}

EsClient::~EsClient()
{
	delete _pQueryMapper;
	delete _pEsSearch;
	delete _pEsIndex;
	delete _pEsDocument;
}

// Search API
Response EsClient::searchQuery(SearchResult& searchResult, std::string index, std::string queryKey,
		std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap)
{
	Response response;

	do
	{
		// QueryMapper로 부터 search query 가져오기
		std::string searchQuery;
		if ( false == _pQueryMapper->getQuery(searchQuery, queryKey, inputParamMap) )
		{
			response.errorType = QUERY_MAPPER_ERROR_TYPE;
			response.errorMessage = QUERY_MAPPER_ERROR;
			response.statusCode = SERVER_INTERNAL_ERROR;
			break;
		}

		// Search API 수행
		SearchResult searchResult;
		response = _pEsSearch->search(searchResult, index, searchQuery);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);

	return response;
}

Response EsClient::searchQuery(SearchResult& searchResult,SearchQuery searchQuery)
{
	Response response;

	return response;
}


// Index API
Response deleteIndex(std::string index)
{
	Response response;

	return response;
}

Response createIndex(std::string index, std::string mappings)
{
	Response response;

	return response;
}

Response existsIndex(std::string index)
{
	Response response;

	return response;
}


// Doucment API
Response deleteDocument(std::string index, std::string docId)
{
	Response response;

	return response;
}

Response createDocument(std::string index, std::string docId)
{
	Response response;

	return response;
}

