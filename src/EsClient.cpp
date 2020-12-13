#include "EsClient.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;

EsClient::EsClient()
{}

EsClient::~EsClient()
{}

void EsClient::test()
{
	XMLDocument doc;
	doc.LoadFile( "test.xml" );
	XMLElement* rootElement = doc.RootElement();
	XMLElement* querymapElement = rootElement->FirstChildElement("querymap");
	while ( querymapElement )
	{
		const XMLAttribute *queryNameAttr = querymapElement->FirstAttribute();
		std::string queryName = "";
		queryName = queryNameAttr->Value();

		const XMLElement* paramElement = querymapElement->FirstChildElement("param");
		const XMLAttribute *paramAttr = paramElement->FirstAttribute();
		std::string strName = "";
		while ( paramAttr )
		{
			printf("param ==> key:%s, value:%s\n", paramAttr->Name(), paramAttr->Value());

			if (strcmp(paramAttr->Name(),"name") == 0)
			{
				strName += ":";
				strName +=  paramAttr->Value();
			}

			paramAttr = paramAttr->Next();
		}
		
		const XMLElement* queryElement = querymapElement->FirstChildElement("query");

		printf("queryMap:%s\n\n %s \n", queryName.c_str(), queryElement->GetText());

		std::string strQuery = queryElement->GetText();
		StringUtil stringUtil;
		std::string result = stringUtil.replaceAll(strQuery, strName.c_str(), "바꾸기완료");
		
		printf("queryMap:%s \n", result.c_str());

		querymapElement = querymapElement->NextSiblingElement();
	}
}

