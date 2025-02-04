/**
* @file EsClient.h
* @brief EsClient Class
*/

#ifndef __ES_CLIENT_H__ 
#define __ES_CLIENT_H__ 

#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>

#include "QueryMapper.h"
#include "Response.h"
#include "EsSearch.h"
#include "EsIndex.h"
#include "EsDocument.h"

namespace es 
{

class EsClient 
{
    public:
        EsClient(std::string host, int port, int timeout, std::string queryPath);
        ~EsClient();

	public:
		// Search API
		Response searchQuery(SearchResult& searchResult, std::string index, std::string query,
				std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap);
		Response searchQuery(SearchResult& searchResult, SearchQuery searchQuery);

		// Index API
		Response deleteIndex(std::string index);
		Response createIndex(std::string index, std::string mappings);
		Response existsIndex(std::string index);

		// Doucment API
		Response deleteDocumentById(std::string index, std::string docId);
		Response deleteDocumentByQuery(std::string index, std::string queryKey,
				std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap);
		Response createDocument(std::string index, std::string queryKey,
				std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap, std::string docId = "");
		Response updateDocument(std::string index, std::string queryKey, 
				std::map<std::string, boost::variant<int, double, std::string>>& inputParamMap, std::string docId);
		Response getDocument(DocResult& docResult, std::string index, std::string docId);

	private:
		std::string _strHost;
		int _nPort;
		int _nTimeout;

		QueryMapper* _pQueryMapper;
		EsSearch* _pEsSearch;
		EsIndex* _pEsIndex;
		EsDocument* _pEsDocument;
};

}

#endif 

