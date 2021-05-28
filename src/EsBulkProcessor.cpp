#include "EsBulkProcessor.h"
#include "util/HttpUtil.h"
#include "util/StringUtil.h"
#include <json/json.h>

using namespace es;

EsBulkProcessor::EsBulkProcessor(std::string host, int port, int timeout)
{
	_host = host;
	_port = port;
	_timeout = timeout;

	_docMaxCount = 1000;			
	_bulkMaxSize = 1024 * 1024LL;		// 1 MB
	_bulkTimeout = 300;					// 5 min

	_bulkDataSize = 0LL;

	_pEsDocument = new EsDocument(_host, _port, _timeout);
}

EsBulkProcessor::~EsBulkProcessor()
{
	delete _pEsDocument;
}

void EsBulkProcessor::setBulkMaxSize(long long int bulkMaxSize)
{
	_bulkMaxSize = bulkMaxSize;
}

void EsBulkProcessor::setDocMaxCount(int docMaxCount)
{
	_docMaxCount = docMaxCount;
}

void EsBulkProcessor::setBulkTimeout(int bulkTimeout)
{
	_bulkTimeout = bulkTimeout;
}


bool EsBulkProcessor::deleteById(std::string index, std::string docId)
{
	if ( _validate(BULK_DELETE_DOC, index, docId) == false )
	{
		return false;
	}

	BulkData bulkData;
	bulkData.index = index;
	bulkData.action = BULK_DELETE_DOC;
	bulkData.docId = docId;

	_bulkDatas.push_back(bulkData);

	return true;
}

bool EsBulkProcessor::update(std::string index, std::string docBody, std::string docId)
{
	if ( _validate(BULK_UPDATE_DOC, index, docId) == false )
	{
		return false;
	}

	BulkData bulkData;
	bulkData.index = index;
	bulkData.action = BULK_UPDATE_DOC;
	bulkData.docBody = docBody;
	bulkData.docId = docId;

	_bulkDataSize += docBody.size();
	_bulkDatas.push_back(bulkData);

	return true;
}



bool EsBulkProcessor::add(std::string index, std::string docBody, std::string docId)
{
	if ( _validate(BULK_CREATE_DOC, index, docId) == false )
	{
		return false;
	}

	BulkData bulkData;
	bulkData.index = index;
	bulkData.action = BULK_CREATE_DOC;
	bulkData.docBody = docBody;
	bulkData.docId = docId;

	_bulkDataSize += docBody.size();
	_bulkDatas.push_back(bulkData);

	return true;
}

bool EsBulkProcessor::_validate(std::string action, std::string index, std::string docId)
{
	if ( action == BULK_UPDATE_DOC || action == BULK_DELETE_DOC )
	{
		// update or delete 는 docId가 필수이다.
		if ( docId.size() == 0 )
		{
			fprintf(stderr, "docId is required for DELETE or UPDATE \n");
			return false;
		}

		// 해당 인덱스에 doc이 존재하지 않는다면 에러
		Response response = _pEsDocument->exists(index, docId);
		if ( response.statusCode != HTTP_SUCCESS )
		{
			fprintf(stderr, "%s(docId) is not exists in %s(index) \n", docId.c_str() , index.c_str());
			return false;
		}
	}

	// maxSize를 넘어섰거나, doc 개수가 maxCount를 넘어서면, bulk API call
	if ( _bulkDataSize > _bulkMaxSize || (int)_bulkDatas.size() > _docMaxCount )
		return false;

	return true;
}

Response EsBulkProcessor::run()
{
	Json::StreamWriterBuilder writeBuilder;
	writeBuilder["commentStyle"] = "None";
	writeBuilder["indentation"] = "";

	Response response;
	std::string strResult = "";

	for ( unsigned int i = 0; i < _bulkDatas.size(); i++ )
	{
		Json::Value jsonResult;
		BulkData bulkData = _bulkDatas[i];

		Json::Value jsonIndex;
		if ( bulkData.docId.size() > 0 )
			jsonIndex["_id"] = bulkData.docId;
		jsonIndex["_index"] = bulkData.index;

		jsonResult[bulkData.action] = jsonIndex;
		std::string strDoc = Json::writeString(writeBuilder, jsonResult);
		strDoc += "\n";

		if ( bulkData.action == BULK_UPDATE_DOC )
		{
			Json::CharReaderBuilder readBuilder;
			Json::CharReader* jsonReader;
			Json::Value docJsonResult;
			std::string err;
			jsonReader = readBuilder.newCharReader();
			jsonReader->parse(bulkData.docBody.c_str(), bulkData.docBody.c_str() + bulkData.docBody.size(), &docJsonResult, &err);
			delete jsonReader;

			Json::Value jsonDoc;
			jsonDoc["doc"] = docJsonResult;
			strDoc += Json::writeString(writeBuilder, jsonDoc);
			strDoc += "\n";
		}
		else if ( bulkData.action == BULK_CREATE_DOC )
		{
			strDoc += bulkData.docBody;
			strDoc += "\n";
		}

		strResult += strDoc;
	}

	// Bulk API call
	HttpUtil httpUtil;
	std::map<std::string, std::string> headers;
	headers["Content-Type"] = "application/json";
	std::string responseBody;
	long responseCode;
	char requestUrl[500];
	sprintf(requestUrl, "%s:%d%s?timeout=%ds", _host.c_str(), _port, BULK_QUERY, _timeout);
	httpUtil.setRequestBody(strResult);

	if ( false == httpUtil.sendHttpRequest(requestUrl, HTTP_POST_METHOD, headers,
				_timeout, responseBody, responseCode) )
	{
		response.errorType = HTTP_REQUEST_ERROR_TYPE;
		response.errorMessage = HTTP_REQUEST_ERROR;
		response.statusCode = SERVER_INTERNAL_ERROR;
		response.body = responseBody;
		return response;
	}

	response.errorType = SUCCESS_TYPE;
	response.errorMessage = SUCCESS;
	response.statusCode = HTTP_SUCCESS;
	response.body = responseBody;

	_bulkDatas.clear();
	_bulkDataSize = 0LL;

	return response;
}


