#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <syncstream>

#include "IErrors.hpp"
#include "Breakthroughs.hpp"
#include "CSVIngest.hpp"

#include "VariantData/DataColumnStore.hpp"
#include "VariantData/ColumnSearch.hpp"

class GameVariant {
private:
	std::filesystem::path* _variant;
	std::filesystem::path* _csvURL;

	std::string _variantName;

	// utility
	IErrors* _error_obj;

	//Datas
	std::shared_ptr<DataColumnStore> _Data;
	bool _data_is_stored = false;
	std::unique_ptr<ColumnSearch> _Search;
	bool _search_is_created = false;

public:
	GameVariant(std::string variant, std::string csvURL, IErrors* error_obj) {
		_variant = new std::filesystem::path(variant);
		_csvURL = new std::filesystem::path(csvURL);
		_variantName = variant;

		// utilities
		_error_obj = error_obj;

		//Setup datas
		_Data = std::make_shared<DataColumnStore>(_error_obj);
	}

	void ingest() {
		std::osyncstream coutStream(std::cout);

		if (_data_is_stored || _search_is_created) {
			_error_obj->setErrorMessage("Already ingested for " + _variantName);
			coutStream << "<--- Error: Already Marked As Ingested for " << _variantName << " --->" << std::endl;
			return;
		}

		if (std::filesystem::exists(*_csvURL))
		{
			CSVIngest csvIngestor = CSVIngest(_csvURL->generic_string(), _error_obj);
			if (!csvIngestor.populateData(_Data.get()))
			{
				coutStream << "Failed to populate data for: " << _csvURL->string() << "\n";
				coutStream << "With error: " << _error_obj->getErrorString() << std::endl;
				return;
			}
		}
		else {
			coutStream << std::string("File ") + _csvURL->string() + " does not exist" << std::endl;
		}
		_data_is_stored = true;

		// Cleanup a little
		delete _variant;
		delete _csvURL;

		// Set the column groups
		_Search = std::make_unique<ColumnSearch>(_error_obj, _Data);
		if (!_Search.get()->initColumnGroups()) {
			coutStream << "Failed to initialise columngroups in Column Search" << std::endl;
			coutStream << "<--- ColumnSearch failed to init with error: " << _error_obj->getErrorString() << " --->" << std::endl;
			return;
		}
		_search_is_created = true;

		coutStream << "<--- Finished Ingesting: " << _variantName << " --> " << std::endl;
	}

	std::shared_ptr< std::vector<uint16_t, uint16_t> > searchWithJson_WithSearch(std::string & inputJson) {
		return nullptr;
	}

	std::shared_ptr< std::vector<uint16_t, uint16_t> > searchWithJson_WithDirect(std::string& inputJson) {
		return nullptr;
	}
};