/**
* @file EsSearch.h
* @brief EsSearch Class
*/

#ifndef __ES_SEARCH_H__ 
#define __ES_SEARCH_H__ 

#include <cstring>
#include <vector>
#include <map>
#include <string>
#include "Response.h"

namespace es
{

class EsSearch 
{
    public:
        EsSearch(std::string host, int port, int timeout);
        ~EsSearch();

		Response search(SearchResult& searchResult, std::string index, std::string query);
		Response search(SearchResult& searchResult,SearchQuery searchQuery);

	private:
		std::string _host;
		int _port;
		int _timeout;
};

}

#endif 

