/**
* @file EsBulkProcessor.h
* @brief EsClient Class
*/

#ifndef __ES_BULK_PROCESSOR_H__ 
#define __ES_BULK_PROCESSOR_H__ 

#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>

#include "QueryMapper.h"
#include "Response.h"

namespace es 
{

class EsBulkProcessor 
{
    public:
        EsBulkProcessor(std::string host, int port, int timeout, std::string queryPath);
        ~EsBulkProcessor();

	public:

	private:
		std::string _strHost;
		int _nPort;
		int _nTimeout;

		QueryMapper* _pQueryMapper;
};

}

#endif 

