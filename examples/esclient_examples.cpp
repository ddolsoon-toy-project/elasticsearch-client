#include <EsClient.h>
#include <iostream>

int main (void)
{
	EsClient esClient;

	esClient.test();

	QueryMapper queryMapper;

	queryMapper.initialize("test.xml");

	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap;
	inputParamMap["distance"] = "5km";
	inputParamMap["latitude"] = 37.5358;
	inputParamMap["longitude"] = 126.9559;

	std::string resultQuery;
	queryMapper.getQuery(resultQuery, "SearchNearestPharmacyFromDistance", inputParamMap);
	printf("SearchNearestPharmacyFromDistance : \n%s\n", resultQuery.c_str());


	std::map<std::string, boost::variant<int, double, std::string>> inputParamMap2;
	inputParamMap2["name"] = "한겨례";
	queryMapper.getQuery(resultQuery, "SearchPharmacyByName", inputParamMap2);
	printf("SearchPharmacyByName: \n%s\n", resultQuery.c_str());

	return 0;
}
