#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <syncstream>

#include <nlohmann/json.hpp>

#include "IErrors.hpp"
#include "Breakthroughs.hpp"
#include "CSVIngest.hpp"

#include "VariantData/DataColumnStore.hpp"
#include "Site/Headers.hpp"

using json = nlohmann::json;

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

		if (_data_is_stored) {
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

		coutStream << "<--- Finished Ingesting: " << _variantName << " --> " << std::endl;
	}

	bool getSiteAsJson(const int siteid, json* retJson) {
		Sites::SiteData retSite;

		if(_Data.get()->getSiteData(siteid, &retSite)) {
			json ret = json({});
			ret["VariantSystem"] = _variantName;
			ret["SiteID"] = siteid;
			ret["Coordinate"] = {
				{"NSNum", retSite.Latitude},
				{"NSChar", retSite.LatNS},
				{"EWNum", retSite.Longitude},
				{"EWChar", retSite.LongEW}
			};
			ret["NamedLandingSYS"] = retSite.MapLocation;
			ret["TopographySYS"] = retSite.MapTopography;
			ret["MapName"] = retSite.MapName;
			ret["Challenge"] = retSite.DifficultyChallenge;
			ret["Altitude"] = retSite.Altitude;
			ret["Temperature"] = retSite.Temperature;

			ret["DustDevils"] = retSite.Disasters.at(0);
			ret["DustStorms"] = retSite.Disasters.at(1);
			ret["Meteors"] = retSite.Disasters.at(2);
			ret["ColdWaves"] = retSite.Disasters.at(3);

			ret["Metals"] = retSite.Resources.at(0);
			ret["RareMetals"] = retSite.Resources.at(1);
			ret["Concrete"] = retSite.Resources.at(2);
			ret["Water"] = retSite.Resources.at(3);

			ret["MissingBreakthroughs"] = 0; // ????????????????????????????????????????????????????????????

			json btrArray = json::array();
			size_t arrayLen = retSite.Breakthroughs.bitset.size();
			for (size_t i = 0; i < arrayLen; i++)
			{
				if (retSite.Breakthroughs.bitset[i]) {
					if (auto retSYS = Breakthroughs::EnumToSYS.find((Breakthroughs::breakthrough_Enum)i); retSYS != Breakthroughs::EnumToSYS.end()) {
						btrArray.emplace_back(retSYS->second);
					}
					else return false;
				}
			}
			ret["Breakthroughs"] = btrArray;

			*retJson = std::move(ret);
			return true;
		}

		return false;
	}
};