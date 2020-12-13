/**
* @file HttpUtil.h
* @brief HttpUtil Class
*/
#ifndef __HTTP_CLIENT_H__ 
#define __HTTP_CLIENT_H__ 

const char* const HTTP_GET_METHOD = "GET";
const char* const HTTP_POST_METHOD = "POST";
const char* const HTTP_DELETE_METHOD = "DELETE";
const char* const HTTP_PATCH_METHOD = "PATCH";

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <curl/curl.h>
#include <cstring>

class HttpUtil 
{
    public:
        HttpUtil();
        ~HttpUtil();

        /**
         * @brief Http 요청을 보내는 함수
         */
		bool sendHttpRequest(const std::string& url, std::string method, 
				const std::map<std::string, std::string> headers, const int& timeOut, std::string& response, long& responseCode);

		void setRequestBody(std::string body);
		static std::string getTrimmedUrl(std::string url);

	private:
		static int _writeCallback(void *ptr, int size, int nmemb, std::string* data);

	private:
		std::string _requestBody;
};

#endif 

