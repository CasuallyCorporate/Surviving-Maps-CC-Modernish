#pragma once

#include <map>
#include <optional>

#include "GameVariant.hpp"

class SearchContext {
private:
	std::map<std::string, std::unique_ptr<GameVariant>>* _mapVariantsPtr;
	GameVariant* _variant;
	std::string _inputJson;

	// Parsed Request JSON

	std::optional<std::string> processRequestJSON() {
		// Parse the JSON Request
		return std::optional<std::string>();
	}
public:
	SearchContext(std::map<std::string, std::unique_ptr<GameVariant>>* variantsPtr) {
		_mapVariantsPtr = variantsPtr;
	};

	void change_context(std::string * inputJson) {
		_inputJson = std::move(inputJson);
	}


};