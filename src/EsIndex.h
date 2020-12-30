/**
* @file EsIndex.h
* @brief EsIndex Class
*/

#ifndef __ES_INDEX_H__ 
#define __ES_INDEX_H__ 

#include <cstring>
#include <vector>
#include <map>
#include <string>
#include "Response.h"

namespace es
{

class EsIndex 
{
    public:
        EsIndex(std::string host, int port, int timeout);
        ~EsIndex();

		Response deleteIndex(std::string index);
		Response createIndex(std::string index, std::string mappings);
		Response existsIndex(std::string index);
	private:
		std::string _host;
		int _port;
		int _timeout;
};

}

#endif 

