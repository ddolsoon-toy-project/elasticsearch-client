#include <elasticsearch/EsClient.h>
#include <elasticsearch/EsBulkProcessor.h>
#include <iostream>

using namespace es;

int main (void)
{
	EsClient esClient("127.0.0.1", 6200, 3000, "test.xml");

	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap;
	inputParamMap["distance"] = "5km";
	inputParamMap["latitude"] = 37.5358;
	inputParamMap["longitude"] = 126.9559;

	SearchResult searchResult;
	Response response = esClient.searchQuery(searchResult, "pet_pharmacy", "SearchNearestPharmacyFromDistance", inputParamMap);
	printf("SearchNearestPharmacyFromDistance : \n%s\n", response.body.c_str());

	/*

	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap2;
	inputParamMap2["name"] = "한겨례";
	Response response2 = esClient.searchQuery(searchResult, "pet_pharmacy", "SearchPharmacyByName", inputParamMap2);
	//printf("SearchPharmacyByName: \n%s\n", resultQuery.c_str());
	
	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap2_1;
	inputParamMap2_1["distance"] = "100000m";
	inputParamMap2_1["latitude"] = 37.5640455;
	inputParamMap2_1["longitude"] = 127.1663142;
	inputParamMap2_1["type"] = "dong";
	inputParamMap2_1["limit"] = 10;
	Response response2_1 = esClient.searchQuery(searchResult, "pet_pharmacy", "GetPharmacyCountGroupByAddress", inputParamMap2_1);
	printf("result.aggs: \n%s\n", searchResult.aggs.c_str());


	return 0;

	Response response3 = esClient.deleteIndex("ddolsoon_test");
	printf("response3 : %d \n", response3.statusCode);
	Response response4 = esClient.createIndex("ddolsoon_test", "{ \"mappings\": { \"properties\": { \"testkey\" : { \"type\": \"keyword\" } } } }");
	printf("response4 : %d \n", response4.statusCode);
	Response response5 = esClient.existsIndex("ddolsoon_test");
	printf("response5 : %d \n", response5.statusCode);

	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap3;
	Response response6 = esClient.createDocument("ddolsoon_test", "CreateDdolsoonDoc", inputParamMap3, "test001");
	printf("response6 : %d \n", response6.statusCode);

	DocResult docResult;

	Response response7 = esClient.getDocument(docResult, "ddolsoon_test", "test001");
	printf("response7 : %d \n", response7.statusCode);

	Response response8 = esClient.deleteDocumentById("ddolsoon_test", "test001");
	printf("response8 : %d \n", response8.statusCode);

	Response response9 = esClient.createDocument("ddolsoon_test", "CreateDdolsoonDoc", inputParamMap3, "test002");
	printf("response9 : %d \n", response9.statusCode);

	Response response10 = esClient.updateDocument("ddolsoon_test", "UpdateDdolsoonDoc", inputParamMap3, "test002");
	printf("response10 : %d \n", response10.statusCode);

	sleep(1);

	//Response response11 = esClient.deleteDocumentByQuery("ddolsoon_test", "DeleteDdolsoonDoc", inputParamMap3);
	//printf("response11 : %d \n", response11.statusCode);

	Response response12 = esClient.getDocument(docResult, "ddolsoon_test", "test002");
	printf("response12 : %s \n", docResult.source.c_str());
	printf("response12 : %d \n", response12.statusCode);

	Response response13 = esClient.createDocument("ddolsoon_test", "CreateDdolsoonDoc", inputParamMap3);
	printf("response13 : %d \n", response13.statusCode);

	// Bulk API 
	EsBulkProcessor bulkProcessor("127.0.0.1", 6200, 3000);
	bulkProcessor.setBulkTimeout(1);

	std::string resultBody = "{ \"ddolsoon\" : \"hello, world!\" }";
	bulkProcessor.deleteById("ddolsoon_idx", "Vfffw3YB2MJt53WmF1pU");

	std::string resultBody2 = "{ \"ddolsoon\" : \"hello, world2222!\" }";
	bulkProcessor.update("ddolsoon_idx", resultBody2, "Vvffw3YB2MJt53WmF1pU");

	for ( int i = 0; i < 10; i++ )
	{
		bulkProcessor.add("ddolsoon_idx", resultBody);
	}
	Response response14 = bulkProcessor.run();
	printf("response14 : %d \n", response14.statusCode);


	*/
	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap15;
	inputParamMap15["channel"] = "pet-pharmacy-platform";
	inputParamMap15["deviceToken"] = "test";
	inputParamMap15["pushYn"] = "Y";
	Response response15 = esClient.createDocument("push_device_info", "CreatePushDeviceInfo", inputParamMap15, "test01");
	printf("response15 : %d \n", response15.statusCode);
	printf("response15 : %s \n", response15.errorMessage.c_str());

	return 0;
}
