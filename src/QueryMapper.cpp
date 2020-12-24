#include <json/json.h>
#include <tinyxml2/tinyxml2.h>

#include "util/HttpUtil.h"
#include "util/StringUtil.h"

#include "QueryMapper.h"
#include "EsClient.h"

using namespace tinyxml2;

QueryMapper::QueryMapper()
{}

QueryMapper::~QueryMapper()
{}

bool QueryMapper::initialize(std::string queryPath)
{	
	XMLDocument doc;
	doc.LoadFile( queryPath.c_str() );
	XMLElement* rootElement = doc.RootElement();
	XMLElement* querymapElement = rootElement->FirstChildElement("querymap");

	while ( querymapElement )
	{
		// queryName
		const XMLAttribute *queryNameAttr = querymapElement->FirstAttribute();
		std::string queryName = "";
		queryName = queryNameAttr->Value();

		// query paramTypeMap
		std::map<std::string, std::string> paramTypeMap;

		const XMLElement* paramElement = querymapElement->FirstChildElement("param");
		while ( paramElement )
		{
			const XMLAttribute *paramAttr = paramElement->FirstAttribute();
			if ( paramAttr == NULL )
			{
				paramElement = paramElement->NextSiblingElement();
				continue;
			}
			std::string strName = "";
			std::string strType = "";
			while ( paramAttr )
			{
				SPDLOG_LOGGER_INFO(g_Logger, "param ==> key:{}, value:{}", paramAttr->Name(), paramAttr->Value());

				if ( strcmp(paramAttr->Name(),"name") == 0 )
				{
					strName = paramAttr->Value();
				}
				else if ( strcmp(paramAttr->Name(), "type") == 0 )
				{
					strType = paramAttr->Value();
				}
				
				paramAttr = paramAttr->Next();
			}

			paramTypeMap[strName] = strType;

			paramElement = paramElement->NextSiblingElement();
		}

		// query
		const XMLElement* queryElement = querymapElement->FirstChildElement("query");
		std::string inputQuery = queryElement->GetText();

		// queryMap 세팅
		Query query;
		query.paramTypeMap = paramTypeMap;
		query.queryDsl = inputQuery;

		queryMap[queryName] = query;

		SPDLOG_LOGGER_INFO(g_Logger, "queryMap:{} {}", queryName.c_str(), queryElement->GetText());

		querymapElement = querymapElement->NextSiblingElement();
	}

	return true;
}

bool QueryMapper::getQuery(std::string& query, const std::string& queryKey,  
		std::map<std::string, boost::variant<int, double, std::string>>& inParamMap)
{
	bool bResult = false;
	std::string strBindQuery = "";

	bResult = _bindInputParam(strBindQuery, queryKey, inParamMap);

	/////////////////////

	query = strBindQuery;

	return bResult;
}

bool QueryMapper::_bindInputParam(std::string& resultQuery, const std::string& queryKey, 
		std::map<std::string, boost::variant<int, double, std::string>>& inParamMap)
{
	// 1.inputParamMap을 순회하면서, queryMap에서 inputTypeMap을 비교한다.
	// key에 해당하는 type이 나오면 해당 type으로 변환한다.
	
	// 2. 변환된 값들에 대해서, query에 바인딩 작업을 한다. (std::string의 경우 "" 로 묶어줌)

	StringUtil stringUtil;
	
	// queryMap에 해당 query가 존재하는지 확인
	std::map<std::string, Query>::iterator queryMapItr;
	std::map<std::string, std::string> paramTypeMap;
	std::string inputQuery;

	SPDLOG_LOGGER_ERROR(g_Logger, "queryMap is size : {}", queryMap.size());
	queryMapItr = queryMap.find(queryKey);
	if (  queryMapItr == queryMap.end() )
	{
		SPDLOG_LOGGER_ERROR(g_Logger, "{} key is not found from queryMap", queryKey);
		return false;
	}

	paramTypeMap = queryMapItr->second.paramTypeMap;
	inputQuery = queryMapItr->second.queryDsl;

	std::map<std::string, std::string>::iterator paramTypeMapItr;
	
	SPDLOG_LOGGER_INFO(g_Logger, "paramTypeMap is size : {}", paramTypeMap.size());
	for ( paramTypeMapItr = paramTypeMap.begin(); paramTypeMapItr != paramTypeMap.end(); paramTypeMapItr++ )
	{
		std::string paramName = paramTypeMapItr->first;
		std::string paramType = paramTypeMapItr->second;

		// paramType 대문자로 변환
		std::transform(paramType.begin(), paramType.end(), paramType.begin(), (int (*)(int))std::toupper);

		std::map<std::string, boost::variant<int, double, std::string>>::iterator inParamMapItr;

		inParamMapItr = inParamMap.find(paramName);
		
		// inParamMap에서 xml에 바인딩된 key가 없다면 에러
		if ( inParamMapItr == inParamMap.end() )
		{
			SPDLOG_LOGGER_ERROR(g_Logger, "{} can't be bound to inParamMap.", paramName);
			continue;
		}

		std::string inParamKey = ":";
		if ( _validateParamType(paramType) == true )
		{
			inParamKey += inParamMapItr->first;
			boost::variant<int, double, std::string> inParam = inParamMapItr->second;
			if ( paramType == INPUT_INT_TYPE )
			{
				int* pIntParam = boost::get<int>(&inParam);
				if  ( pIntParam == nullptr )
				{
					SPDLOG_LOGGER_ERROR(g_Logger, "{} type mismatch", inParamMapItr->first);
					continue;
				}

				std::string strIntParam = std::to_string(*pIntParam);
				inputQuery = stringUtil.replaceAll(inputQuery, inParamKey, strIntParam);
			}
			else if ( paramType == INPUT_STRING_TYPE )
			{
				std::string* pStrParam = boost::get<std::string>(&inParam);
				if  ( pStrParam == nullptr )
				{
					SPDLOG_LOGGER_ERROR(g_Logger, "{} type mismatch", inParamMapItr->first);
					continue;
				}

				std::string strStrParam = "\"";
				strStrParam += *pStrParam;
				strStrParam += "\"";

				inputQuery = stringUtil.replaceAll(inputQuery, inParamKey, strStrParam);
			}
			else if ( paramType == INPUT_DOUBLE_TYPE )
			{
				double* pDoubleParam = boost::get<double>(&inParam);
				if  ( pDoubleParam == nullptr )
				{
					SPDLOG_LOGGER_ERROR(g_Logger, "{} type mismatch", inParamMapItr->first);
					continue;
				}

				std::string strDoubleParam = std::to_string(*pDoubleParam);
				inputQuery = stringUtil.replaceAll(inputQuery, inParamKey, strDoubleParam);
			}
		}
		else
		{
			SPDLOG_LOGGER_ERROR(g_Logger, "{} type not exists.", paramType);
		}

		SPDLOG_LOGGER_INFO(g_Logger, "param : {} , {}", paramTypeMapItr->first, paramTypeMapItr->second);
	}

	resultQuery = inputQuery;
	
	return true;
}

bool QueryMapper::_validateParamType(std::string paramType)
{
	if ( paramType == INPUT_INT_TYPE || paramType == INPUT_STRING_TYPE ||
			paramType == INPUT_DOUBLE_TYPE )
	{
		return true;
	}
	else
	{
		return false;
	}
}

