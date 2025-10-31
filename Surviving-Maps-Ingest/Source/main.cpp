#include <iostream>
#include <syncstream>
#include <memory>
#include <vector>
#include <thread>
#include <functional>
#include <chrono>

#include "IErrors.hpp"
#include "GameVariant.hpp"

/* Main
*  - Holds the variants themselves in memory
*  - Starts CSV ingests
*  - provides boost::beast search capability
*/

std::map<std::string, std::unique_ptr<GameVariant>> _variantMap;
IErrors _error_obj;

std::vector<std::thread> _ingestThreads;

int main(/*int args, char* argv[]*/) {
	std::cout << "<!! Setting variants !!>\n";

	// Insert all variants to process
	_variantMap["STANDARD"] = std::make_unique<GameVariant>("STANDARD", "CSVMapData/MapData Breakthroughs.csv", &_error_obj);
	_variantMap["GREEN_PLANET"] = std::make_unique<GameVariant>("GREEN_PLANET", "CSVMapData/MapData Breakthroughs GP.csv", &_error_obj);
	_variantMap["BELOW_BEYOND"] = std::make_unique<GameVariant>("BELOW_BEYOND", "CSVMapData/MapData Breakthroughs BB.csv", &_error_obj);
	_variantMap["BEYOND_GREEN"] = std::make_unique<GameVariant>("BEYOND_GREEN", "CSVMapData/MapData Breakthroughs GP BB.csv", &_error_obj);
	_variantMap["TITO_GREEN_PLANET"] = std::make_unique<GameVariant>("TITO_GREEN_PLANET", "CSVMapData/MapData Tito GP.csv", &_error_obj);
	_variantMap["EVANS_GREEN_PLANET"] = std::make_unique<GameVariant>("EVANS_GREEN_PLANET", "CSVMapData/MapData Evans GP.csv", &_error_obj);

	std::cout << "<!! Starting variants' ingest !!>\n";
	// process all map data into their variants
	std::map<std::string, std::unique_ptr<GameVariant>>::iterator it;
	for (it = _variantMap.begin(); it != _variantMap.end(); it++) {
		// Ingest CSV
		std::cout << "<--- Ingesting " << it->first << " -->" << std::endl;
		_ingestThreads.emplace_back(
			std::thread(
				&GameVariant::ingest,
				it->second.get()
			)
		);
	}
	while (!_ingestThreads.empty()) {
		_ingestThreads.back().join();
		_ingestThreads.pop_back();
	}
	if (_error_obj.hasErrored()) {
		return -1;
	}

	std::cout << "End of data ingest and formatting" << std::endl;

	std::cout << "Testing search timings" << std::endl;

	std::string inputJson = "";

	auto standardIt = _variantMap.find("STANDARD");

	auto beforeSearch = std::chrono::high_resolution_clock::now();
	standardIt->second.get()->searchWithJson_WithSearch(inputJson);
	auto afterSearch = std::chrono::high_resolution_clock::now();
	auto beforeDirectSearch = std::chrono::high_resolution_clock::now();
	standardIt->second.get()->searchWithJson_WithDirect(inputJson);
	auto afterDirectSearch = std::chrono::high_resolution_clock::now();

	auto normalSearch = afterSearch.time_since_epoch() - beforeSearch.time_since_epoch();
	auto directSearch = afterDirectSearch.time_since_epoch() - beforeDirectSearch.time_since_epoch();

	std::cout << "Normal Search: " << std::chrono::duration_cast<std::chrono::nanoseconds>(normalSearch).count() << std::endl;
	std::cout << "Direct Search: " << std::chrono::duration_cast<std::chrono::nanoseconds>(directSearch).count() << std::endl;
}