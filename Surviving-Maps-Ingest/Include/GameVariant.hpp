#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <syncstream>
#include <optional>

#include <nlohmann/json.hpp>

#include "IErrors.hpp"
#include "Breakthroughs.hpp"
#include "CSVIngest.hpp"

#include "VariantData/DataColumnStore.hpp"
#include "VariantData/RequestData.hpp"
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

	// Json gets
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

	bool convertToValueComparitor(std::optional<RequestData::ValueComparitor>* ret, std::string itemName, json& itemObject) {
		int retNum = 0;
		if (auto itemres = itemObject.find(itemName); itemres != itemObject.end()) {
			if (auto numres = itemres.value().find("Number"); numres != itemres.value().end()) {
				if (!numres.value().is_number_integer()) return false;
				retNum = numres.value();
			} else return false;
			if (auto compres = itemres.value().find("comparitor"); compres != itemres.value().end()) {
				std::optional<RequestData::Comparitor> compopt = RequestData::getComparitor(compres.value());
				if (compopt) {
					*ret = { retNum, compopt.value() };
					return true;
				} else return false;
			}
		}

		return false;
	}

	bool getPageAsJson(const json* request, json* retJson) {
		
		// GetPageNum
		std::optional<int> page = {};
		if (auto resp = request->find("PageNum"); resp != request->end()) {
			if (!resp.value().is_number_integer()) return false;
			page = resp.value();
		}

		// GetSorting
		std::optional<RequestData::Sorting> sorting = {};
		if (auto resp = request->find("Sorting"); resp != request->end()) {
			Header::Headers itemHdr = Header::Headers::End;

			if (auto itemres = resp.value().find("SortItem"); itemres != resp.value().end()) {
				if (!itemres.value().is_string()) return false;
				if (auto headerres = Header::csvToHeader.find(itemres.value()); headerres != Header::csvToHeader.end()) {
					itemHdr = headerres->second;
				} else return false;
			} else return false;

			if (auto lowres = resp.value().find("isLowest"); lowres != resp.value().end()) {
				if (!lowres.value().is_boolean()) return false;
				sorting = {itemHdr, lowres.value()};
			}
		}

		// simple:
		RequestData::PageSimple pgSimple;
		json simple;
		if (auto ressimple = request->find("Simple"); ressimple != request->end()) {
			if (!ressimple.value().is_object()) return false;
			simple = ressimple.value();
		} else return false;

		// get Disasters num & comparitor
		convertToValueComparitor(&pgSimple.Disasters, "Disasters", simple);

		// get Resources num & comparitor
		convertToValueComparitor(&pgSimple.Resources, "Resources", simple);

		// complex:
		std::optional<RequestData::PageComplex> pgComplex;
		std::optional<json> complex;
		if (auto rescomplex = request->find("Complex"); rescomplex != request->end()) {
			if (!rescomplex.value().is_object()) return false;
			complex = rescomplex.value();
		}

		if (complex) {
			if (auto compDis = complex.value().find("Disasters"); compDis != complex.value().end()) {
				RequestData::ComplexDisasters retDis;
				// get from Disasters

				convertToValueComparitor(&retDis.ColdWaves, "ColdWaves", compDis.value());
				convertToValueComparitor(&retDis.DustDevils, "DustDevils", compDis.value());
				convertToValueComparitor(&retDis.DustStorms, "DustStorms", compDis.value());
				convertToValueComparitor(&retDis.Meteors, "Meteors", compDis.value());

				if (retDis.ColdWaves || retDis.DustDevils || retDis.DustStorms || retDis.Meteors) {
					pgComplex = { retDis, {} };
				} else return false;
			}
			if (auto compRes = complex.value().find("Resources"); compRes != complex.value().end()) {
				RequestData::ComplexResources retRes;
				// get from Resources

				convertToValueComparitor(&retRes.Concrete, "Concrete", compRes.value());
				convertToValueComparitor(&retRes.Metal, "Metal", compRes.value());
				convertToValueComparitor(&retRes.RareMetal, "RareMetal", compRes.value());
				convertToValueComparitor(&retRes.Water, "Water", compRes.value());

				if (retRes.Concrete || retRes.Metal || retRes.RareMetal || retRes.Water) {
					if (pgComplex) {
						pgComplex.value().Resources = retRes;
					}
					else {
						pgComplex = { {}, retRes };
					}
				} else return false;
			}
			else {
				if (!pgComplex.value().Disasters) return false;
			}
		}

		// Do the search

		return false;
	}
};