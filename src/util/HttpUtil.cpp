#include "HttpUtil.h"
#include "json/json.h"

using namespace es;

HttpUtil::HttpUtil()
{}

HttpUtil::~HttpUtil()
{}

bool HttpUtil::sendHttpRequest(const std::string& url, std::string method,
		const std::map<std::string, std::string> headers, const int& timeOut, std::string& response, long& responseCode)
{
	CURL *curlCtx = curl_easy_init();
	if ( curlCtx == NULL )
	{
		fprintf(stderr, "curlCtx initialize failed \n");
		return false;
	}

	bool bResult = true;
	do
	{
		// URL 세팅
		curl_easy_setopt(curlCtx, CURLOPT_URL, url.c_str());

		// Method 세팅
		std::transform(method.begin(), method.end(), method.begin(), toupper); 
		if ( method == HTTP_GET_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_HTTPGET, 1L);
		}
		else if ( method == HTTP_POST_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_POST, 1L);
		}
		else if ( method == HTTP_DELETE_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_CUSTOMREQUEST, "DELETE");
		}
		else if ( method == HTTP_PATCH_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_CUSTOMREQUEST, "PATCH");
		}
		else if ( method == HTTP_PUT_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_CUSTOMREQUEST, "PUT");
		}
		else if ( method == HTTP_HEAD_METHOD )
		{
			curl_easy_setopt(curlCtx, CURLOPT_CUSTOMREQUEST, "HEAD");
		}
		else
		{
			fprintf(stderr, "not supported method\n");
			bResult = false;
			break;
		}

		// Headers 세팅
		char header[200];
		struct curl_slist *curlList = NULL;
		std::map<std::string,std::string>::const_iterator headersItr = headers.begin();
		for ( ; headersItr != headers.end(); headersItr++ )
		{
			sprintf(header, "%s: %s", headersItr->first.c_str(), headersItr->second.c_str());
			curlList = curl_slist_append(curlList, header);
			memset(header, 0x00, sizeof(header)); 
		}

		if ( curlList != NULL )
		{
			curl_easy_setopt(curlCtx, CURLOPT_HTTPHEADER, curlList);
		}

		// timeout 세팅
		curl_easy_setopt(curlCtx, CURLOPT_CONNECTTIMEOUT, timeOut);

		// body 세팅 (optional)
		if ( _requestBody.size() > 0 )
		{
			curl_easy_setopt(curlCtx, CURLOPT_POSTFIELDS, _requestBody.c_str());
		}

		// output 세팅
		if ( method == HTTP_HEAD_METHOD ) // HEAD일경우, body 없음
		{
			curl_easy_setopt(curlCtx, CURLOPT_NOBODY, 1);
		}
		else
		{
			curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, _writeCallback);
			curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, &response);
		}

		CURLcode res = curl_easy_perform(curlCtx);
		if ( res != CURLE_OK )
		{
			fprintf(stderr, "http request failed: %s \n", curl_easy_strerror(res));
			bResult = false;
			break;
		}

		curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &responseCode);
		if ( responseCode < 200 || responseCode > 299 )
		{
			fprintf(stderr, "http responseCode is not 2xx (%ld|%s) \n", responseCode, response.c_str());
			bResult = false;
			break;
		}
	}
	while (false);

	curl_easy_cleanup(curlCtx);

	return bResult;



}

int HttpUtil::_writeCallback(void *ptr, int size, int nmemb, std::string* data)
{
	data->append((char*) ptr, size * nmemb);
	return size * nmemb;
}

void HttpUtil::setRequestBody(std::string body)
{
	_requestBody = body;
}

std::string HttpUtil::getTrimmedUrl(std::string url)
{
	// http://${url} 
	
	// 마지막 '/' 제거
	if ( url[url.size() - 1] == '/' )
	{
		url.erase(url.length() -1, 1);
	}

	// http:// 이후의, "//"는 "/"로 교체
	
	int startPos = 7;
	while ( (startPos = url.find("//", startPos)) != -1 )
	{
		url.replace(startPos, 2, "/");
		startPos += 1;
	}

	return url;
}



