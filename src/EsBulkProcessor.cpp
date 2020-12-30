#include "EsBulkProcessor.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"

using namespace tinyxml2;
using namespace es;

EsBulkProcessor::EsBulkProcessor(std::string host, int port, int timeout, std::string queryPath)
{
	_strHost = host;
	_nPort = port;
	_nTimeout = timeout;
	_pQueryMapper = new QueryMapper(queryPath);

	// QueryMapper 초기화
	_pQueryMapper->initialize(queryPath);
}

EsBulkProcessor::~EsBulkProcessor()
{
	delete _pQueryMapper;
}

