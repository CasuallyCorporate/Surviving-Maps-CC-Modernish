#pragma once

#include <map>
#include <optional>

#include <nlohmann/json.hpp>

#include "GameVariant.hpp"

using json = nlohmann::json;

class SearchContext {
private:
	std::map<std::string, std::unique_ptr<GameVariant>>* _mapVariantsPtr;
	GameVariant* _variant;
	json* _inputJson;

	// Parsed Request JSON

	std::optional<std::string> processRequestJSON() {
		// Parse the JSON Request
		return std::optional<std::string>();
	}
public:
	SearchContext(
		std::map<std::string, std::unique_ptr<GameVariant>>* variantsPtr,
		json* inputJson
	){
		_inputJson = inputJson;
		_mapVariantsPtr = variantsPtr;
	};

	json runSearch() {
		return json({});
	};
};