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

#include "Response.h"
#include "EsDocument.h"

const char* const BULK_CREATE_DOC = "create";
const char* const BULK_UPDATE_DOC = "update";
const char* const BULK_DELETE_DOC = "delete";

typedef struct _BulkData
{
	std::string index;
	std::string action;
	std::string docBody;
	std::string docId;
} BulkData;

namespace es 
{

class EsBulkProcessor 
{
    public:
        EsBulkProcessor(std::string host, int port, int timeout);
        ~EsBulkProcessor();

		void setBulkMaxSize(long long int bulkMaxSize);
		void setDocMaxCount(int docMaxCount);
		void setBulkTimeout(int bulkTimeout);

		bool add(std::string index, std::string docBody, std::string docId = "");
		bool deleteById(std::string index, std::string docId);
		bool update(std::string index, std::string docBody, std::string docId);

		Response run();

	private:
		bool _validate(std::string action, std::string index, std::string docId);

	private:
		std::string _host;
		int _port;
		int _timeout;

		int _docMaxCount;
		long long int _bulkMaxSize;
		int _bulkTimeout;
		std::vector<BulkData> _bulkDatas;
		long long int _bulkDataSize;

		EsDocument* _pEsDocument;
};

}

#endif 

