/**
* @file EsClient.h
* @brief EsClient Class
*/

#ifndef __ES_CLIENT_H__ 
#define __ES_CLIENT_H__ 

#include <iostream>
#include <cstring>
#include <string>
#include <spdlog/spdlog.h>

#include "QueryMapper.h"
#include "EsSearch.h"

extern std::shared_ptr<spdlog::logger> g_Logger;

class EsClient 
{
    public:
        EsClient();
        ~EsClient();

	public:

	private:
		std::string _strHost;
		int _nPort;
		int _nTimeout;

		QueryMapper* _pQueryMapper;
		EsSearch* _pEsSearch;
};

#endif 

