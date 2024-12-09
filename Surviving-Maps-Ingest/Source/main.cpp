#include <iostream>
#include <map>
#include <memory>

#include "IErrors.hpp"

#include "Breakthroughs.hpp"
#include "GameVariant.hpp"

/* Main
*  - Holds the variants themselves in memory
*  - Starts CSV ingests
*  - starts processing the data into the sub-searchable data formats
*/

std::map<std::string, std::unique_ptr<GameVariant>> _variantMap;

int main(int args, char* argv[]) {
	// Insert all variants to process
	_variantMap["STANDARD"] = std::make_unique<GameVariant>("STANDARD", "MapData Breakthroughs.csv");
	_variantMap["GREEN_PLANET"] = std::make_unique<GameVariant>("GREEN_PLANET", "MapData Breakthroughs GP.csv");
	_variantMap["BELOW_BEYOND"] = std::make_unique<GameVariant>("BELOW_BEYOND", "MapData Breakthroughs BB.csv");
	_variantMap["BEYOND_GREEN"] = std::make_unique<GameVariant>("BEYOND_GREEN", "MapData Breakthroughs GP BB.csv");
	_variantMap["TITO_GREEN_PLANET"] = std::make_unique<GameVariant>("TITO_GREEN_PLANET", "MapData Tito GP.csv");
	_variantMap["EVANS_GREEN_PLANET"] = std::make_unique<GameVariant>("EVANS_GREEN_PLANET", "MapData Evans GP.csv");

	// For each variant
	std::map<std::string, std::unique_ptr<GameVariant>>::iterator it;
	for (it = _variantMap.begin(); it != _variantMap.end(); it++) {
		// Ingest CSV
		if (!it->second.get()->ingestCSV()) {
			// some kind of error
			Errors::ErrorMessage error = it->second.get()->getErrorPublic();
			std::cerr << "CSV Error: " << error.error << " To:" + Errors::errorToFromtoString.at(error.to) << " From:" << Errors::errorToFromtoString.at(error.from) << std::endl;
			return -1;
		}
		// Process datas
		if (!it->second.get()->processDatas())
		{
			// some kind of error
			Errors::ErrorMessage error = it->second.get()->getErrorPublic();
			std::cerr << "Datas Error: " << error.error << " To:" + Errors::errorToFromtoString.at(error.to) << " From:" << Errors::errorToFromtoString.at(error.from) << std::endl;
		}
		// Delete variant (to save RAM)
		it->second.~unique_ptr();
	}

	std::cout << "End of data ingest, formattign and output" << std::endl;

	// Crude Unit Testing:
}