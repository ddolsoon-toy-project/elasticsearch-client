#include "EsClient.h"
#include <json/json.h>
#include <tinyxml2/tinyxml2.h>
#include "util/HttpUtil.h"
#include "util/StringUtil.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>

using namespace tinyxml2;

std::shared_ptr<spdlog::logger> g_Logger = spdlog::daily_logger_mt("esclient", "logs/esclient.log", 0, 0);

EsClient::EsClient()
{
	g_Logger->set_pattern("%g:%#|%Y-%m-%dT%H:%M:%S|%l|%n|%v|tid:%t");
	g_Logger->flush_on(spdlog::level::info);
	printf("EsClient::EsClient()\n");
}

EsClient::~EsClient()
{}
