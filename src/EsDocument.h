/**
* @file EsDocument.h
* @brief EsDocument Class
*/

#ifndef __ES_DOCUMENT_H__ 
#define __ES_DOCUMENT_H__ 

#include <cstring>
#include <vector>
#include <map>
#include <string>
#include "Response.h"

namespace es
{

class EsDocument 
{
    public:
        EsDocument(std::string host, int port, int timeout);
        ~EsDocument();

		Response deleteById(std::string index, std::string docId);
		Response deleteByQuery(std::string index, std::string query);
		Response create(std::string index, std::string query, std::string docId = "");
		Response update(std::string index, std::string query, std::string docId);
		Response get(DocResult& docResult, std::string index, std::string docId);
		Response exists(std::string index, std::string docId);

	private:
		std::string _host;
		int _port;
		int _timeout;
};

}

#endif 

