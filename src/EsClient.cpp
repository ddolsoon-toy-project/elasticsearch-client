#include "EsClient.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"


using namespace tinyxml2;
using namespace es;

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
Response EsClient::deleteIndex(std::string index)
{
	Response response;

	do
	{
		response = _pEsIndex->deleteIndex(index);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);


	return response;
}

Response EsClient::createIndex(std::string index, std::string mappings)
{
	Response response;

	do
	{
		response = _pEsIndex->createIndex(index, mappings);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);

	return response;
}

Response EsClient::existsIndex(std::string index)
{
	Response response;

	do
	{
		response = _pEsIndex->existsIndex(index);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);

	return response;
}

// Doucment API
Response EsClient::deleteDocumentById(std::string index, std::string docId)
{
	Response response;

	do
	{
		response = _pEsDocument->deleteById(index, docId);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);


	return response;
}

Response EsClient::deleteDocumentByQuery(std::string index, std::string queryKey,
		std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap)
{
	Response response;

	do
	{
		// QueryMapper로 부터 delete query 가져오기
		std::string deleteQuery;
		if ( false == _pQueryMapper->getQuery(deleteQuery, queryKey, inputParamMap) )
		{
			response.errorType = QUERY_MAPPER_ERROR_TYPE;
			response.errorMessage = QUERY_MAPPER_ERROR;
			response.statusCode = SERVER_INTERNAL_ERROR;
			break;
		}

		// Delete API 수행
		response = _pEsDocument->deleteByQuery(index, deleteQuery);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);

	return response;
}

Response EsClient::createDocument(std::string index, std::string queryKey, 
		std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap, std::string docId)
{
	Response response;

	do
	{
		// QueryMapper로 부터 create query 가져오기
		std::string createQuery;
		if ( false == _pQueryMapper->getQuery(createQuery, queryKey, inputParamMap) )
		{
			response.errorType = QUERY_MAPPER_ERROR_TYPE;
			response.errorMessage = QUERY_MAPPER_ERROR;
			response.statusCode = SERVER_INTERNAL_ERROR;
			break;
		}

		// Create API 수행
		response = _pEsDocument->create(index, createQuery, docId);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);


	return response;

}

Response EsClient::updateDocument(std::string index, std::string queryKey, 
		std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap, std::string docId)
{
	Response response;

	do
	{
		// QueryMapper로 부터 update query 가져오기
		std::string updateQuery;
		if ( false == _pQueryMapper->getQuery(updateQuery, queryKey, inputParamMap) )
		{
			response.errorType = QUERY_MAPPER_ERROR_TYPE;
			response.errorMessage = QUERY_MAPPER_ERROR;
			response.statusCode = SERVER_INTERNAL_ERROR;
			break;
		}

		// Create API 수행
		response = _pEsDocument->update(index, updateQuery, docId);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);

	return response;
}

Response EsClient::getDocument(DocResult& docResult, std::string index, std::string docId)
{
	Response response;

	do
	{
		// Search API 수행
		response = _pEsDocument->get(docResult, index, docId);
		if ( response.statusCode != 201 || response.statusCode != 200 )
		{
			break;
		}

	} while (false);


	return response;
}
