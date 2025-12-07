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
			if (!csvIngestor.populateData(_Data.get(), &coutStream))
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
			for (Breakthroughs::breakthrough_Enum btrenum : retSite.BreakthroughOrder) {
				if (auto retSYS = Breakthroughs::EnumToSYS.find(btrenum); retSYS != Breakthroughs::EnumToSYS.end()) {
					btrArray.emplace_back(retSYS->second);
				}
			}
			ret["Breakthroughs"] = btrArray;

			*retJson = std::move(ret);
			return true;
		}

		return false;
	}

	bool convertToValueComparitor(std::optional<RequestData::ValueRequest>* ret, std::string itemName, json& itemObject) {
		int retNum = 0;
		if (auto itemres = itemObject.find(itemName); itemres != itemObject.end()) {
			if (auto numres = itemres.value().find("Number"); numres != itemres.value().end()) {
				if (!numres.value().is_number_integer()) return false;
				retNum = numres.value();
			} else return false;
			if (auto compres = itemres.value().find("comparitor"); compres != itemres.value().end()) {
				auto compopt = RequestData::getComparitor(compres.value());
				if (compopt) {
					*ret = { retNum, compopt.value() };
					return true;
				} else return false;
			}
		}

		return false;
	}

	/// <summary>
	/// RequestJson parsed in the order in which they appear on the website
	/// </summary>
	/// <param name="request">pointer to request json</param>
	/// <param name="retJson">pointer to returned json</param>
	/// <returns>bool indicates succes</returns>
	bool getPageAsJson(const json* request, json* retJson, std::string** error_ptr_ptr) {

		// simple:
		RequestData::PageSimple pgSimple;
		json simple;
		if (auto ressimple = request->find("Simple"); ressimple != request->end()) {
			if (!ressimple.value().is_object()) return false;
			simple = ressimple.value();
		}
		else return false;

		// get simple Disasters num & comparitor
		convertToValueComparitor(&pgSimple.Disasters, "Disasters", simple);

		// get simple Resources num & comparitor
		convertToValueComparitor(&pgSimple.Resources, "Resources", simple);

		// Breakthrough filters
		std::optional<std::set<Breakthroughs::breakthrough_Enum>> breakthroughFilters = {};
		if (auto btrSimple = simple.find("BreakthroughFilters"); btrSimple != simple.end()) {
			if (!btrSimple.value().is_array()) return false;
			for (json& btr : btrSimple.value())
			{
				if (!btr.is_string()) return false;
				if (auto enumRes = Breakthroughs::SYSToEnum.find(btr); enumRes != Breakthroughs::SYSToEnum.end()) {
					if (!breakthroughFilters) {
						breakthroughFilters = std::set<Breakthroughs::breakthrough_Enum>();
					}
					breakthroughFilters.value().insert(enumRes->second);
				}
				else return false;
			}
		}

		// complex:
		std::optional<RequestData::PageComplex> pgComplex;
		std::optional<json> complex;
		if (auto rescomplex = request->find("Complex"); rescomplex != request->end()) {
			if (!rescomplex.value().is_object()) return false;
			complex = rescomplex.value();
		}

		if (complex) {
			RequestData::PageComplex tmpComplex = RequestData::PageComplex();

			if (auto compRes = complex.value().find("Resources"); compRes != complex.value().end()) {
				RequestData::ComplexResources retRes;
				// get from Resources

				convertToValueComparitor(&retRes.Concrete, "Concrete", compRes.value());
				convertToValueComparitor(&retRes.Metal, "Metal", compRes.value());
				convertToValueComparitor(&retRes.RareMetal, "RareMetal", compRes.value());
				convertToValueComparitor(&retRes.Water, "Water", compRes.value());

				if (retRes.Concrete || retRes.Metal || retRes.RareMetal || retRes.Water) {
					tmpComplex.Resources = retRes;
				}
				else return false;
			}
			if (auto compDis = complex.value().find("Disasters"); compDis != complex.value().end()) {
				RequestData::ComplexDisasters retDis;
				// get from Disasters

				convertToValueComparitor(&retDis.ColdWaves, "ColdWaves", compDis.value());
				convertToValueComparitor(&retDis.DustDevils, "DustDevils", compDis.value());
				convertToValueComparitor(&retDis.DustStorms, "DustStorms", compDis.value());
				convertToValueComparitor(&retDis.Meteors, "Meteors", compDis.value());

				if (retDis.ColdWaves || retDis.DustDevils || retDis.DustStorms || retDis.Meteors) {
					tmpComplex.Disasters = retDis;
				} else return false;
			}
			if (auto compMap = complex.value().find("Map"); compMap != complex.value().end())
			{
				if (!compMap.value().is_object()) return false;
				if (auto challRes = compMap.value().find("Challenge"); challRes != compMap.value().end()) {
					convertToValueComparitor(&tmpComplex.MapChallenge, "Challenge", compMap.value());
				}
				if (auto mnRes = compMap.value().find("MapName"); mnRes != compMap.value().end()) {
					if (!mnRes.value().is_array()) return false;
					for (auto nameIT = mnRes.value().begin(); nameIT != mnRes.value().end(); ++nameIT) {
						if (!tmpComplex.MapNames) {
							tmpComplex.MapNames = std::vector<std::string>();
							tmpComplex.MapNames.value().reserve(mnRes.value().size());
						}
						if (nameIT.value().is_string()) {
							tmpComplex.MapNames.value().emplace_back(nameIT.value());
						} else return false;
					}
				}
				if (auto naRes = compMap.value().find("NamedArea"); naRes != compMap.value().end()) {
					if (!naRes.value().is_array()) return false;
					for (auto nameIT = naRes.value().begin(); nameIT != naRes.value().end(); ++nameIT) {
						if (!tmpComplex.MapNamedAreas) {
							tmpComplex.MapNamedAreas = std::vector<std::string>();
							tmpComplex.MapNamedAreas.value().reserve(naRes.value().size());
						}
						if (nameIT.value().is_string()) {
							tmpComplex.MapNamedAreas.value().emplace_back(nameIT.value());
						}
						else return false;
					}
				}
				if (auto topRes = compMap.value().find("Topography"); topRes != compMap.value().end()) {
					if (!topRes.value().is_array()) return false;
					for (auto nameIT = topRes.value().begin(); nameIT != topRes.value().end(); ++nameIT) {
						if (!tmpComplex.MapTopographies) {
							tmpComplex.MapTopographies = std::vector<std::string>();
							tmpComplex.MapTopographies.value().reserve(topRes.value().size());
						}
						if (nameIT.value().is_string()) {
							tmpComplex.MapTopographies.value().emplace_back(nameIT.value());
						}
						else return false;
					}
				}
			}

			if (tmpComplex.Resources || tmpComplex.Disasters || tmpComplex.MapChallenge ||
				tmpComplex.MapNames || tmpComplex.MapNamedAreas || tmpComplex.MapTopographies) {
				pgComplex = std::move(tmpComplex);
			}
			else return false;
		}

		// GetSorting
		std::optional<RequestData::SortingRequest> sorting = {};
		if (auto resp = request->find("Sorting"); resp != request->end()) {
			RequestData::SortingRequest tmpSort;

			if (auto itemres = resp.value().find("SortItem"); itemres != resp.value().end()) {
				if (!itemres.value().is_string()) return false;
				if (itemres.value() == "Location") {
					tmpSort.SortCategory = RequestData::SortingCategory::Location_Coord;
				}
				else if (itemres.value() == "Resources") {
					tmpSort.SortCategory = RequestData::SortingCategory::ResourcesTotal;
				}
				else if (itemres.value() == "Disasters") {
					tmpSort.SortCategory = RequestData::SortingCategory::DisastersTotal;
				}
				else if (auto headerres = Header::csvToHeader.find(itemres.value()); headerres != Header::csvToHeader.end()) {
					tmpSort.SortItem = headerres->second;
				}
				else return false;
			}
			else return false;

			if (auto lowres = resp.value().find("isLowest"); lowres != resp.value().end()) {
				if (!lowres.value().is_boolean()) return false;
				tmpSort.isLowest = lowres.value();
				sorting = std::move(tmpSort);
			}
		}

		// GetPageNum
		std::optional<int> page = {};
		if (auto resp = request->find("PageNum"); resp != request->end()) {
			if (!resp.value().is_number_integer()) return false;
			page = resp.value();
		}

		// Do the search
		return _Data->searchPageData(
			retJson, error_ptr_ptr,
			_variantName,
			breakthroughFilters,
			pgSimple,
			pgComplex,
			sorting,
			page
		);
	}

	bool getCoordAsJson(const RequestData::CoordRequest* request, json* retJson, std::string** error_ptr_ptr) {
		int siteid = 0;
		if (_Data->getSiteIDFromCoord(request, &siteid)) {
			// create/return the json
			if (getSiteAsJson(siteid, retJson)) {
				return true;
			}
			else {
				*error_ptr_ptr = &RequestResponse::ErrorProcessing;
				return false;
			}
		}
		else {
			*error_ptr_ptr = &RequestResponse::ErrorReqDataInvalid;
			return false;
		}

	}
};