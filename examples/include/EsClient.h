/**
* @file EsClient.h
* @brief EsClient Class
*/

#ifndef __ES_CLIENT_H__ 
#define __ES_CLIENT_H__ 

#include <cstring>
#include <string>

class EsClient 
{
    public:
        EsClient();
        ~EsClient();
		void test();

	private:
		std::string _host;
};

#endif 

