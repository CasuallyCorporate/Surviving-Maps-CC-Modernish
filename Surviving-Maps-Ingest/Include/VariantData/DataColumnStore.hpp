#pragma once

#include <array>
#include <vector>
#include <set>

#include <nlohmann/json.hpp>

#include "Site/Headers.hpp"
#include "RequestData.hpp"
#include "RequestResponse.hpp"

using json = nlohmann::json;

class IndexMover {
private:
	std::vector<uint16_t>* _vecTot;
	std::vector<uint16_t> _inner;
	uint16_t _maxIndex;
	std::vector<uint16_t>::iterator _it;
	uint16_t _nonIt = 0;
	bool _hasMoved = false, _isIncremental = false;
public:
	IndexMover(std::vector<uint16_t>* vecTot, uint16_t maxIndex) {
		_vecTot = vecTot;
		_maxIndex = maxIndex;
		_it = _vecTot->begin();
		_isIncremental = (_it == _vecTot->end());
	}

	void move(uint16_t value) {
		_inner.emplace_back(value);
		_hasMoved = true;
	}

	bool nextValue(uint16_t* retValue) {
		if (_isIncremental) {
			// Go through until max
			if (_nonIt < _maxIndex) {
				*retValue = _nonIt;
				_nonIt++;
				return true;
			}
			return false;
		}
		else {
			if (_it != _vecTot->end()) {
				*retValue = *_it;
				_it++;
				return true;
			}
			return false;
		}
	}

	bool checkMovedReconcileReset() {

		*_vecTot = _inner;

		// Reset and return
		if (_hasMoved) {
			_it = _vecTot->begin();
			_isIncremental = (_it == _vecTot->end());
			_inner.clear();
			_nonIt = 0;

			return true;
		}
		return false;
	}
};

class DataColumnStore {
private:
	// utility
	IErrors* _error_obj;

	size_t _currIndex = 0;
	size_t _maxEntries = 0;
	bool _sitesSet = false;

	// Latitude
	char* _LatitudeNS;
	int* _Latitiude;

	// Longitude
	char* _LongitudeEW;
	int* _Longitude;

	// MapName
	std::vector<std::string>* _MapNameTemporary;
	uint8_t* _MapNameDictIndices;

	// Named Location
	std::vector<std::string>* _NamedLocationTemporary;
	uint8_t* _NamedLocationDictIndices;

	// Topography
	std::vector<std::string>* _TopographyTemporary;
	uint8_t* _TopographyDictIndices;

	// Difficulty
	std::vector<int>* _DifficultyTemporary;
	uint8_t* _DifficultyDictIndices;

	int* _Altitude;
	int* _Temperature;

	// Basic Resources
	uint8_t* _Metals;
	uint8_t* _RareMetals;
	uint8_t* _Concrete;
	uint8_t* _Water;

	// Basic Disasters
	uint8_t* _DustDevils;
	uint8_t* _DustStorms;
	uint8_t* _Meteors;
	uint8_t* _ColdWaves;

	// Breaktthoughs
	Breakthroughs::btrData* _Breakthoughs;

	// default "every index" vector
	std::vector<uint16_t> _everyIndex;

	template <class t>
	bool ValuesToDictVals(std::set<t>* dict, std::vector<t>* inLocation, uint8_t* outLocation) {
		if (dict->size() < UINT8_MAX) {
		}
		else {
			_error_obj->setErrorMessage("Dictionary has more entries than uint8_MAX");
			return false;
		}

		size_t inLen = inLocation->size();
		for (size_t i = 0; i < inLen; i++)
		{
			auto search = dict->find(inLocation->at(i));
			if (search != dict->end()) {
				outLocation[i] = std::distance(dict->begin(), search);
			}
			else {
				_error_obj->setErrorMessage("Dictionary did not include value");
				return false;
			}
		}

		return true;
	}

public:
	std::set<std::string> MapNameDictionary;
	std::set<std::string> NamedLocationDictionary;
	std::set<std::string> TopographyDictionary;
	std::set<int> DifficultyDictionary;

	DataColumnStore(IErrors* error_obj) {
		// utilities
		_error_obj = error_obj;
	}
	~DataColumnStore() {
		// Latitude
		delete _LatitudeNS;
		delete _Latitiude;

		// Longitude
		delete _LongitudeEW;
		delete _Longitude;

		// MapName
		delete _MapNameDictIndices;

		// Named Location
		delete _NamedLocationDictIndices;

		// Topography
		delete _TopographyDictIndices;

		// Difficulty
		delete _DifficultyDictIndices;

		delete _Altitude;
		delete _Temperature;

		// Basic Resources
		delete _Metals;
		delete _RareMetals;
		delete _Concrete;
		delete _Water;

		// Basic Disasters
		delete _DustDevils;
		delete _DustStorms;
		delete _Meteors;
		delete _ColdWaves;

		// Breaktthoughs
		delete _Breakthoughs;
	}

	bool setCollectionMax(size_t totalEntries) {
		_maxEntries = totalEntries;

		try {
			_LatitudeNS = new char[_maxEntries];
			_Latitiude = new int[_maxEntries];

			_LongitudeEW = new char[_maxEntries];
			_Longitude = new int[_maxEntries];

			_TopographyTemporary = new std::vector<std::string>();
			_TopographyDictIndices = new uint8_t[_maxEntries];

			_DifficultyTemporary = new std::vector<int>();
			_DifficultyDictIndices = new uint8_t[_maxEntries];

			_Altitude = new int[_maxEntries];
			_Temperature = new int[_maxEntries];

			_Metals = new uint8_t[_maxEntries];
			_RareMetals = new uint8_t[_maxEntries];
			_Concrete = new uint8_t[_maxEntries];
			_Water = new uint8_t[_maxEntries];

			_DustDevils = new uint8_t[_maxEntries];
			_DustStorms = new uint8_t[_maxEntries];
			_Meteors = new uint8_t[_maxEntries];;
			_ColdWaves = new uint8_t[_maxEntries];

			_MapNameTemporary = new std::vector<std::string>();
			_MapNameDictIndices = new uint8_t[_maxEntries];

			_NamedLocationTemporary = new std::vector<std::string>();
			_NamedLocationDictIndices = new uint8_t[_maxEntries];

			_Breakthoughs = new Breakthroughs::btrData[_maxEntries];

			_everyIndex.reserve(_maxEntries);
			for (uint16_t i = 0; i < _maxEntries; i++)
			{
				_everyIndex.emplace_back(i);
			}
		}
		catch (...) {
			_error_obj->setErrorMessage("Unable to allocate. May be out of memory");
			return false;
		}

		return true;
	}

	bool addSite(Sites::Site* tmpSite) {
		if (_sitesSet) {
			_error_obj->setErrorMessage("Sites already set. No more sites can be added");
			return false;
		}
		if (_currIndex < _maxEntries) {
		}
		else {
			_error_obj->setErrorMessage("Current index has reached or exceeded max sites");
			return false;
		}

		_LatitudeNS[_currIndex] = tmpSite->data.LatNS;
		_Latitiude[_currIndex] = tmpSite->data.Latitude;

		_LongitudeEW[_currIndex] = tmpSite->data.LongEW;
		_Longitude[_currIndex] = tmpSite->data.Longitude;

		_MapNameTemporary->push_back(tmpSite->data.MapName);
		MapNameDictionary.insert(tmpSite->data.MapName);

		_NamedLocationTemporary->push_back(tmpSite->data.MapLocation);
		NamedLocationDictionary.insert(tmpSite->data.MapLocation);

		_TopographyTemporary->push_back(tmpSite->data.MapTopography);
		TopographyDictionary.insert(tmpSite->data.MapTopography);

		_DifficultyTemporary->push_back(tmpSite->data.DifficultyChallenge);
		DifficultyDictionary.insert(tmpSite->data.DifficultyChallenge);

		_Altitude[_currIndex] = tmpSite->data.Altitude;

		_Temperature[_currIndex] = tmpSite->data.Temperature;

		_Metals[_currIndex] = tmpSite->data.Resources[0];
		_RareMetals[_currIndex] = tmpSite->data.Resources[1];
		_Concrete[_currIndex] = tmpSite->data.Resources[2];
		_Water[_currIndex] = tmpSite->data.Resources[3];

		_DustDevils[_currIndex] = tmpSite->data.Disasters[0];
		_DustStorms[_currIndex] = tmpSite->data.Disasters[1];
		_Meteors[_currIndex] = tmpSite->data.Disasters[2];
		_ColdWaves[_currIndex] = tmpSite->data.Disasters[3];

		_Breakthoughs[_currIndex] = Breakthroughs::btrData{};
		Breakthroughs::btrData::setBreakthroughSet(&_Breakthoughs[_currIndex], &tmpSite->_cachedBreakthroughs);

		_currIndex++;
		return true;
	}

	bool setEndOfData() {
		_sitesSet = true;

		// Also interpetet all values into their appropriate dictionary index
		// Map name
		if (ValuesToDictVals(&MapNameDictionary, _MapNameTemporary, _MapNameDictIndices)) {
			delete _MapNameTemporary;
			_MapNameTemporary = nullptr;
		} else {
			return false;
		}
		// Map location
		if (ValuesToDictVals(&NamedLocationDictionary, _NamedLocationTemporary, _NamedLocationDictIndices)) {
			delete _NamedLocationTemporary;
			_NamedLocationTemporary = nullptr;
		} else {
			return false;
		}
		// Topography
		if (ValuesToDictVals(&TopographyDictionary, _TopographyTemporary, _TopographyDictIndices)) {
			delete _TopographyTemporary;
			_TopographyTemporary = nullptr;
		}
		else {
			return false;
		}
		// Difficulty
		if (ValuesToDictVals(&DifficultyDictionary, _DifficultyTemporary, _DifficultyDictIndices)) {
			delete _DifficultyTemporary;
			_DifficultyTemporary = nullptr;
		}
		else {
			return false;
		}

		return true;
	}

	bool getSiteData(size_t SiteIndex, Sites::SiteData* tmpSite) {
		if (SiteIndex < _maxEntries) {
		}
		else {
			return false;
		}
		tmpSite->Latitude = _Latitiude[SiteIndex];
		tmpSite->LatNS = _LatitudeNS[SiteIndex];

		tmpSite->Longitude = _Longitude[SiteIndex];
		tmpSite->LongEW = _LongitudeEW[SiteIndex];

		tmpSite->MapName = *std::next(
			MapNameDictionary.begin(),
			_MapNameDictIndices[SiteIndex]);

		tmpSite->MapLocation = *std::next(
			NamedLocationDictionary.begin(),
			_NamedLocationDictIndices[SiteIndex]);

		tmpSite->MapTopography = *std::next(
			TopographyDictionary.begin(),
			_TopographyDictIndices[SiteIndex]);

		tmpSite->DifficultyChallenge = *std::next(
			DifficultyDictionary.begin(),
			_DifficultyDictIndices[SiteIndex]);

		tmpSite->Altitude = _Altitude[SiteIndex];

		tmpSite->Temperature = _Temperature[SiteIndex];

		tmpSite->Resources[0] = _Metals[SiteIndex];
		tmpSite->Resources[1] = _RareMetals[SiteIndex];
		tmpSite->Resources[2] = _Concrete[SiteIndex];
		tmpSite->Resources[3] = _Water[SiteIndex];

		tmpSite->Disasters[0] = _DustDevils[SiteIndex];
		tmpSite->Disasters[1] = _DustStorms[SiteIndex];
		tmpSite->Disasters[2] = _Meteors[SiteIndex];
		tmpSite->Disasters[3] = _ColdWaves[SiteIndex];

		tmpSite->Breakthroughs = _Breakthoughs[SiteIndex];

		return true;
	}

	size_t getMaxSites() {
		return _maxEntries;
	}

	// Search the data
	bool searchData(
		json* retJson, std::string** error_ptr_ptr,
		std::string variantName,
		std::optional<std::set<Breakthroughs::breakthrough_Enum>> breakthroughFilters,
		RequestData::PageSimple pgSimple,
		std::optional<RequestData::PageComplex> pgComplex,
		std::optional<RequestData::SortingRequest> sorting,
		std::optional<int> page
		) {
		std::vector<uint16_t> valueIndexes;
		uint16_t siteID = 0;
		// Search from most diverse data source to least

		// mover helper
		IndexMover mover = IndexMover(&valueIndexes, _maxEntries);

		// Breakthroughs
		if (breakthroughFilters) {
			Breakthroughs::btrData* sitebtrs = nullptr;
			bool bitsset = true;
			for (uint16_t i = 0; i < _maxEntries; i++)
			{
				sitebtrs = &_Breakthoughs[i];
				for (auto btr : breakthroughFilters.value()) {
					if (!sitebtrs->bitset.test(btr)) {
						bitsset = false;
						break;
					}
				}
				if (bitsset) {
					// add to pile
					mover.move(i);
				}
				else {
					// reset
					bitsset = true;
					// implicit continue
				}
			}
			if (!mover.checkMovedReconcileReset()) {
				*error_ptr_ptr = &RequestResponse::ErrorNoResults;
				return false;
			}
		}

		// simple resources
		if (pgSimple.Resources || pgSimple.Disasters) {
			if (pgSimple.Resources) {
				uint8_t total = 0;

				while (mover.nextValue(&siteID)) {
					total = 0;
					total += _Metals[siteID];
					total += _RareMetals[siteID];
					total += _Concrete[siteID];
					total += _Water[siteID];

					switch (pgSimple.Resources.value().comparitor)
					{
					case RequestData::LessEqual:
						if (total <= pgSimple.Resources.value().value) {
							mover.move(siteID);
						}
						break;
					case RequestData::Equal:
						if (total == pgSimple.Resources.value().value) {
							mover.move(siteID);
						}
						break;
					case RequestData::MoreEqual:
						if (total >= pgSimple.Resources.value().value) {
							mover.move(siteID);
						}
						break;
					}
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
			if (pgSimple.Disasters) {
				uint8_t total = 0;
				

				while (mover.nextValue(&siteID)) {
					total = 0;
					total += _DustStorms[siteID];
					total += _DustDevils[siteID];
					total += _Meteors[siteID];
					total += _ColdWaves[siteID];

					switch (pgSimple.Disasters.value().comparitor)
					{
					case RequestData::LessEqual:
						if (total <= pgSimple.Disasters.value().value) {
							mover.move(siteID);
						}
						break;
					case RequestData::Equal:
						if (total == pgSimple.Disasters.value().value) {
							mover.move(siteID);
						}
						break;
					case RequestData::MoreEqual:
						if (total >= pgSimple.Disasters.value().value) {
							mover.move(siteID);
						}
						break;
					}
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
		}
		else if (pgComplex){
			if (pgComplex.value().Resources) {
				RequestData::ComplexResources* compRes = &pgComplex.value().Resources.value();

				if (compRes->Metal) {
					while (mover.nextValue(&siteID)) {
						switch (compRes->Metal.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Metals[siteID] <= compRes->Metal.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Metals[siteID] == compRes->Metal.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Metals[siteID] >= compRes->Metal.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compRes->RareMetal) {
					while (mover.nextValue(&siteID)) {
						switch (compRes->RareMetal.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_RareMetals[siteID] <= compRes->RareMetal.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_RareMetals[siteID] == compRes->RareMetal.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_RareMetals[siteID] >= compRes->RareMetal.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compRes->Concrete) {
					while (mover.nextValue(&siteID)) {
						switch (compRes->Concrete.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Concrete[siteID] <= compRes->Concrete.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Concrete[siteID] == compRes->Concrete.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Concrete[siteID] >= compRes->Concrete.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compRes->Water) {
					while (mover.nextValue(&siteID)) {
						switch (compRes->Water.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Water[siteID] <= compRes->Water.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Water[siteID] == compRes->Water.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Water[siteID] >= compRes->Water.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
			}
			if (pgComplex.value().Disasters) {
				RequestData::ComplexDisasters* compDis = &pgComplex.value().Disasters.value();

				if (compDis->DustDevils) {
					while (mover.nextValue(&siteID)) {
						switch (compDis->DustDevils.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Water[siteID] <= compDis->DustDevils.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Water[siteID] == compDis->DustDevils.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Water[siteID] >= compDis->DustDevils.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compDis->DustStorms) {
					while (mover.nextValue(&siteID)) {
						switch (compDis->DustStorms.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Water[siteID] <= compDis->DustStorms.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Water[siteID] == compDis->DustStorms.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Water[siteID] >= compDis->DustStorms.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compDis->Meteors) {
					while (mover.nextValue(&siteID)) {
						switch (compDis->Meteors.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Water[siteID] <= compDis->Meteors.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Water[siteID] == compDis->Meteors.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Water[siteID] >= compDis->Meteors.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
				if (compDis->ColdWaves) {
					while (mover.nextValue(&siteID)) {
						switch (compDis->ColdWaves.value().comparitor)
						{
						case RequestData::LessEqual:
							if (_Water[siteID] <= compDis->ColdWaves.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							if (_Water[siteID] == compDis->ColdWaves.value().value) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							if (_Water[siteID] >= compDis->ColdWaves.value().value) {
								mover.move(siteID);
							}
							break;
						}
					}

					if (!mover.checkMovedReconcileReset()) {
						*error_ptr_ptr = &RequestResponse::ErrorNoResults;
						return false;
					}
				}
			}
			if (pgComplex.value().MapChallenge) {
				if (auto diffIt = DifficultyDictionary.find(pgComplex.value().MapChallenge.value().value); diffIt != DifficultyDictionary.end()) {
					int reqValue = *diffIt;

					while (mover.nextValue(&siteID)) {
						switch (pgComplex.value().MapChallenge.value().comparitor)
						{
						case RequestData::LessEqual:
							diffIt = DifficultyDictionary.begin();
							std::advance(diffIt, _DifficultyDictIndices[siteID]);
							if (*diffIt <= reqValue) {
								mover.move(siteID);
							}
							break;
						case RequestData::Equal:
							diffIt = DifficultyDictionary.begin();
							std::advance(diffIt, _DifficultyDictIndices[siteID]);
							if (*diffIt == reqValue) {
								mover.move(siteID);
							}
							break;
						case RequestData::MoreEqual:
							diffIt = DifficultyDictionary.begin();
							std::advance(diffIt, _DifficultyDictIndices[siteID]);
							if (*diffIt >= reqValue) {
								mover.move(siteID);
							}
							break;
						}
					}
				}
				else {
					*error_ptr_ptr = &RequestResponse::ErrorReqDataInvalid;
					return false;
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
			if (pgComplex.value().MapNames) {
				std::set<uint8_t> validIndexes;
				for (auto name : pgComplex.value().MapNames.value()) {
					if (auto nameRes = MapNameDictionary.find(name); nameRes != MapNameDictionary.end()) {
						validIndexes.emplace(std::distance(MapNameDictionary.begin(), nameRes));
					}
					else {
						*error_ptr_ptr = &RequestResponse::ErrorReqDataInvalid;
						return false;
					}
				}

				while (mover.nextValue(&siteID)) {
					if (validIndexes.contains(_MapNameDictIndices[siteID])) {
						mover.move(siteID);
					}
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
			if (pgComplex.value().MapNamedAreas) {
				std::set<uint8_t> validIndexes;
				for (auto name : pgComplex.value().MapNamedAreas.value()) {
					if (auto nameRes = NamedLocationDictionary.find(name); nameRes != NamedLocationDictionary.end()) {
						validIndexes.emplace(std::distance(NamedLocationDictionary.begin(), nameRes));
					}
					else {
						*error_ptr_ptr = &RequestResponse::ErrorReqDataInvalid;
						return false;
					}
				}

				while (mover.nextValue(&siteID)) {
					if (validIndexes.contains(_NamedLocationDictIndices[siteID])) {
						mover.move(siteID);
					}
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
			if (pgComplex.value().MapTopographies) {
				std::set<uint8_t> validIndexes;
				for (auto name : pgComplex.value().MapTopographies.value()) {
					if (auto nameRes = TopographyDictionary.find(name); nameRes != TopographyDictionary.end()) {
						validIndexes.emplace(std::distance(TopographyDictionary.begin(), nameRes));
					}
					else {
						*error_ptr_ptr = &RequestResponse::ErrorReqDataInvalid;
						return false;
					}
				}

				while (mover.nextValue(&siteID)) {
					if (validIndexes.contains(_TopographyDictIndices[siteID])) {
						mover.move(siteID);
					}
				}

				if (!mover.checkMovedReconcileReset()) {
					*error_ptr_ptr = &RequestResponse::ErrorNoResults;
					return false;
				}
			}
		}

		// check for no valueIndexes, presume variant only. Get everything
		if (valueIndexes.size() == 0) {
			valueIndexes = _everyIndex;
		}

		// Before sorting <----------- CACHING ------------>

		// End with sorting
		if (sorting) {

		}

		// Split the results into 20 item pages
		uint16_t pages = 0, remvalues = valueIndexes.size();
		while (remvalues > 20) {
			pages++;
			remvalues -= 20;
		}
		if (remvalues > 0) {
			pages++;
		}
		pages++;

		uint16_t pagereq = 1;
		std::pair<uint16_t, uint16_t> focused_range = { 0,20 };

		// Get specific page?
		if (page) {
			pagereq = page.value();
			// generate specific page json
			if (page.value() > 0 && page.value() <= pages) {
				focused_range.first = (page.value() - 1) * 20;
				if (focused_range.first + 20 > valueIndexes.size()) {
					focused_range.second = valueIndexes.size();
				}
				else {
					focused_range.second = focused_range.first + 20;
				}
			}
			else {
				*error_ptr_ptr = &RequestResponse::ErrorProcessing;
				return false;
			}
		}

		// generate page json
		json returnPage;
		returnPage["VariantSystem"] = variantName;
		returnPage["PageNum"] = pagereq;
		returnPage["PageTot"] = pages;
		json pageSites = json::array({});
		Sites::SiteData tmpSite;
		json tmpSiteObject = json::object({});
		uint16_t siteIndex = 0;
		for (size_t i = focused_range.first; i <= focused_range.second; i++)
		{
			try
			{
				siteIndex = valueIndexes.at(i);
			}
			catch (...)
			{
				*error_ptr_ptr = &RequestResponse::ErrorProcessing;
				return false;
			}
			if (this->getSiteData(siteIndex, &tmpSite)) {
				tmpSiteObject["ID"] = siteIndex;
				std::string coordinate;
				coordinate += std::to_string(tmpSite.Latitude);
				coordinate += tmpSite.LatNS;
				coordinate += ":";
				coordinate += std::to_string(tmpSite.Longitude);
				coordinate += tmpSite.LongEW;
				tmpSiteObject["Coordinates"] = coordinate;
				tmpSiteObject["DisastersTot"] = tmpSite.Disasters[0] + tmpSite.Disasters[1] + tmpSite.Disasters[2] + tmpSite.Disasters[3]; // DustDevils, DustStorms, Meteors, ColdWaves
				tmpSiteObject["DustDevils"] = tmpSite.Disasters[0];
				tmpSiteObject["DustStorms"] = tmpSite.Disasters[1];
				tmpSiteObject["Meteors"] = tmpSite.Disasters[2];
				tmpSiteObject["ColdWaves"] = tmpSite.Disasters[3];
				tmpSiteObject["ResourcesTot"] = tmpSite.Resources[0] + tmpSite.Resources[1] + tmpSite.Resources[2] + tmpSite.Resources[3]; // Metals, RareMetals, Concrete, Water
				tmpSiteObject["Metal"] = tmpSite.Resources[0];
				tmpSiteObject["RareMetal"] = tmpSite.Resources[1];
				tmpSiteObject["Concrete"] = tmpSite.Resources[2];
				tmpSiteObject["Water"] = tmpSite.Resources[3];
			}
			else {
				*error_ptr_ptr = &RequestResponse::ErrorProcessing;
				return false;
			}
			pageSites.emplace_back(tmpSiteObject);
		}
		returnPage["PageSites"] = pageSites;

		*retJson = returnPage;
		return true;
	}
};