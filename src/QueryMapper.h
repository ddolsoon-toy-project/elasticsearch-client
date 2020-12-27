/**
* @file QueryMapper.h
* @brief QueryMapper Class
*/

#ifndef __QUERY_MAPPER_H__ 
#define __QUERY_MAPPER_H__ 

#include <cctype>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <boost/variant.hpp>
#include <boost/format.hpp>

#include "Response.h"

const char* const INPUT_INT_TYPE = "INT";
const char* const INPUT_STRING_TYPE = "STRING";
const char* const INPUT_DOUBLE_TYPE = "DOUBLE";

typedef struct _Query
{
	std::map<std::string, std::string> paramTypeMap;
	std::string queryDsl;
}Query;

namespace es
{

class QueryMapper 
{
    public:
        QueryMapper(std::string queryPath);
        ~QueryMapper();

		/**
		 * queryMap
		 *
		 * ex) MapSearchQuery, { "key1": "value1", "key2": value2 }
		 */
		std::map<std::string, Query> queryMap;

	public:

		bool initialize(std::string queryPath);


		// 바인딩된 쿼리를 가져온다.
		bool getQuery(std::string& query, const std::string& queryKey,  
				std::map<std::string, boost::variant<int, double, std::string>>& inParamMap);

	private:
		std::string _queryPath;

	private:
		// 해당쿼리의 input parameter로 바인딩을 수행한다.
		bool _bindInputParam(std::string& resultQuery,  const std::string& queryKey,
				std::map<std::string, boost::variant<int, double, std::string>>& inParamMap);

		bool _validateParamType(std::string paramType);
};

}

#endif 

