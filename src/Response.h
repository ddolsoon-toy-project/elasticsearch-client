/**
* @file Response.h
* @brief Response Class
*/

#ifndef __RESPONSE_H__ 
#define __RESPONSE_H__ 

#include <cctype>
#include <cstring>
#include <map>
#include <vector>
#include <iostream>
#include <boost/variant.hpp>

const char* const SUCCESS_TYPE = "success";
const char* const QUERY_MAPPER_ERROR_TYPE = "QueryMapper mapping error";
const char* const HTTP_REQUEST_ERROR_TYPE = "HTTP Request error";
const char* const JSON_PARSING_ERROR_TYPE = "Json Parsing error";

const char* const QUERY_MAPPER_ERROR = "QueryMapper mapping error occurred.";
const char* const HTTP_REQUEST_ERROR = "Request error occurred.";
const char* const JSON_PARSING_ERROR = "Json Parsing error occurred.";
const char* const SUCCESS = "success";

const char* const SEARCH_QUERY = "/_search";
const char* const DOCUMENT_QUERY = "/_doc";
const char* const DOCUMENT_DELETE_BY_QUERY = "/_delete_by_query";
const char* const BULK_QUERY = "/_bulk";

#define SERVER_INTERNAL_ERROR 500
#define HTTP_SUCCESS 200

typedef struct _Response
{
	std::string errorType;
	std::string errorMessage;
	int statusCode;
}Response;

typedef struct _SearchResult
{
	int totalCount;
	double maxScore;
	std::string hits;
	std::string aggs;
}SearchResult;

typedef struct _DocResult 
{
	std::string id;
	int version;
	std::string source;
}DocResult;


typedef struct _SearchQuery
{
	std::string index;	// 검색 index
	std::string query;	// 검색 query
	int from;			// 검색 from
	int size;			// 검색 size
}SearchQuery;

#endif 

