#pragma once

#include <memory>
#include <vector>
#include <set>

#include "IErrors.hpp"
#include "VariantData/DataColumnStore.hpp"
#include "Site/Site.hpp"

template <typename T> class columnNumGroup {
public:
	std::vector<uint16_t> startIndices;
	std::vector<uint16_t> endIndices;

	std::set<T> includedValues;

	T maxValue, minValue;

	columnNumGroup(size_t startIndexValue, T startValue)
	{
		startIndices.emplace_back(startIndexValue);
		endIndices.emplace_back(startIndexValue);
		includedValues.insert(startValue);
		maxValue = startValue;
		minValue = startValue;
	}

	bool checkValCanExist(T val) {
		if (val <= maxValue && minValue <= val) {
			if (includedValues.find(val) != includedValues.end()) {
				return true;
			}
		}
		return false;
	}
};

class btrGroup {
public:
	std::vector<uint16_t> startIndices;
	std::vector<uint16_t> endIndices;

	Breakthroughs::btrData Breakthroughs;

	btrGroup(size_t startIndexValue, Breakthroughs::btrData data) :
		Breakthroughs(data) {
		startIndices.emplace_back(startIndexValue);
		endIndices.emplace_back(startIndexValue);
	}
};

class ColumnSearch {
private:
	bool initialised = false;
	IErrors* _error_obj;
	std::shared_ptr<DataColumnStore> _dataStore;

	// Column groupings
	// const std::vector<columnNumGroup<char>> _LatitudeNSGroups = {}; // Cannot group effectively
	std::vector<columnNumGroup<int>> _Latitiude; // Numbers are grouped

	// const std::vector<columnNumGroup<char>> _LongitudeEWGroups = {}; // Cannot group effectively
	std::vector<columnNumGroup<int>> _Longitude; // Numbers possible. Increments

	std::vector<columnNumGroup<uint8_t>> _TopographyGroups;

	std::vector<columnNumGroup<uint8_t>> _DififcultyGroups; // Some groupings. Need to see...

	// const std::vector<columnNumGroup<int>> _AltitudeGroups = {}; // too variable to group
	// const std::vector<columnNumGroup<int>> _TemperatureGroups = {}; // weird and variable

	std::vector<columnNumGroup<uint8_t>> _MetalsGroups;
	std::vector<columnNumGroup<uint8_t>> _RareMetalsGroups;
	std::vector<columnNumGroup<uint8_t>> _ConcreteGroups;
	std::vector<columnNumGroup<uint8_t>> _WaterGroups;

	std::vector<columnNumGroup<uint8_t>> _DustDevilGroups;
	std::vector<columnNumGroup<uint8_t>> _DustStormGroups;
	std::vector<columnNumGroup<uint8_t>> _MeteorGroups;
	std::vector<columnNumGroup<uint8_t>> _ColdWaveGroups;

	std::vector<columnNumGroup<uint8_t>> _MapNameGroups;
	std::vector<columnNumGroup<uint8_t>> _NamedLocationGroups;

	std::vector<btrGroup> _BreakthroughGroups;

	bool handleColumnIntStretches(std::vector<columnNumGroup<int>>& colGroupVec, size_t& index, int& value) {
		if (colGroupVec.back().maxValue == value) {
			colGroupVec.back().endIndices.at(0) = index;
			return true;
		}
		else {
			colGroupVec.emplace_back(index, value);
			return true;
		}
		return false;
	}
	bool handleColumnIntIncrementers(std::vector<columnNumGroup<int>>& colGroupVec, size_t& index, int& value) {
		int distance = colGroupVec.back().minValue - value;
		if (distance > -11 && distance < 11) {

			if (colGroupVec.back().maxValue < value) {
				colGroupVec.back().maxValue = value;
			}
			
			colGroupVec.back().includedValues.insert(value);
			colGroupVec.back().endIndices.at(0) = index;

			if (colGroupVec.back().minValue > value) colGroupVec.back().minValue = value;
			if (colGroupVec.back().maxValue < value) colGroupVec.back().maxValue = value;
			return true;
		}
		else {
			colGroupVec.emplace_back(index, value);
			return true;
		}
		return false;
	}

	bool handleColumnDictionaryGroups(std::vector<columnNumGroup<uint8_t>>& colGroupVec, size_t grouping, size_t& index, uint8_t& value) {
		if (colGroupVec.back().includedValues.size() < grouping) {
			colGroupVec.back().includedValues.insert(value);
			colGroupVec.back().endIndices.at(0) = index;

			if (colGroupVec.back().minValue > value) colGroupVec.back().minValue = value;
			if (colGroupVec.back().maxValue < value) colGroupVec.back().maxValue = value;
			return true;
		}
		else {
			if (colGroupVec.back().includedValues.find(value) != colGroupVec.back().includedValues.end()) {
				colGroupVec.back().endIndices.at(0) = index;
				return true;
			}
		}

		colGroupVec.emplace_back(index, value);
		return true;
	}

	bool handleColumnUintGroups(std::vector<columnNumGroup<uint8_t>>& colGroupVec, size_t& index, uint8_t& value) {
		if (colGroupVec.back().includedValues.size() < 2) {
			colGroupVec.back().includedValues.insert(value);
			colGroupVec.back().endIndices.at(0) = index;

			if (colGroupVec.back().minValue > value) colGroupVec.back().minValue = value;
			if (colGroupVec.back().maxValue < value) colGroupVec.back().maxValue = value;
			return true;
		}
		if (colGroupVec.back().includedValues.find(value) != colGroupVec.back().includedValues.end()) {
			colGroupVec.back().endIndices.at(0) = index;
			return true;
		}

		colGroupVec.emplace_back(index, value);
		return true;
	}

	bool handleBreakthroughGroups(std::vector<btrGroup>& colGroupVec, size_t& index, Breakthroughs::btrData& values) {
		auto sameSet = values.bitset;
		sameSet = sameSet & colGroupVec.back().Breakthroughs.bitset;

		size_t diff = values.bitset.count() - sameSet.count();

		if (colGroupVec.back().Breakthroughs.bitset.count() + diff > 50) {
			colGroupVec.emplace_back(index, values);
			return true;
		}
		else {
			// integrate
			colGroupVec.back().Breakthroughs.bitset = colGroupVec.back().Breakthroughs.bitset | values.bitset;
			colGroupVec.back().endIndices.at(0) = index;
			return true;
		}

		return false;
	}

	size_t collapseGroupings(std::vector<columnNumGroup<uint8_t>> & groupings) {
		std::vector<size_t> found;
		size_t index = 0;
		size_t all_found = 0;

		while (index < groupings.size())
		{
			for (size_t i = index; i < groupings.size(); i++)
			{
				if (index == i) continue;

				if (groupings.at(index).minValue == groupings.at(i).minValue &&
					groupings.at(index).maxValue == groupings.at(i).maxValue) {
					if (groupings.at(index).includedValues == groupings.at(i).includedValues) {
						found.push_back(i);
						groupings.at(index).startIndices.push_back(groupings.at(i).startIndices.at(0));
						groupings.at(index).endIndices.push_back(groupings.at(i).endIndices.at(0));
					}
				}
			}

			all_found += found.size();

			// Delete found in reverse
			size_t popped;
			while (found.size() > 0) {
				popped = found.back();
				groupings.erase(groupings.begin() + popped);
				found.pop_back();
			}

			groupings.at(index).startIndices.shrink_to_fit();
			groupings.at(index).endIndices.shrink_to_fit();

			index++;
		}

		groupings.shrink_to_fit();
		return all_found;
	}

	size_t collapseBreakthroughGroups(std::vector<btrGroup> &groups) {
		std::vector<size_t> found;
		size_t index = 0;
		size_t all_found = 0;

		while (index < groups.size()) {
			for (size_t i = index; i < groups.size(); i++)
			{
				if (index == i) continue;

				// Bitset comparison
				size_t btrIndex = 0;
				bool bitsSame = true;
				while (btrIndex < groups.at(index).Breakthroughs.bitset.size()) {
					if (groups.at(index).Breakthroughs.bitset[btrIndex] !=
						groups.at(i).Breakthroughs.bitset[btrIndex]) {
						bitsSame = false;
						break;
					}
					btrIndex++;
				}

				if (bitsSame) {
					found.push_back(i);
					groups.at(index).startIndices.push_back(groups.at(i).startIndices.at(0));
					groups.at(index).endIndices.push_back(groups.at(i).endIndices.at(0));
				}
			}

			all_found += found.size();

			// Delete found in reverse
			size_t popped;
			while (found.size() > 0) {
				popped = found.back();
				groups.erase(groups.begin() + popped);
				found.pop_back();
			}

			groups.at(index).startIndices.shrink_to_fit();
			groups.at(index).endIndices.shrink_to_fit();

			index++;
		}

		groups.shrink_to_fit();
		return all_found;
	}

public:
	ColumnSearch(IErrors* error_object, std::shared_ptr<DataColumnStore> dataStore) {
		_error_obj = error_object;
		_dataStore = dataStore;
	}

	bool initColumnGroups() {
		// Create intermediary groupings for each column, that help to search the data
		if (initialised) {
			_error_obj->setErrorMessage("Init called, but already initialised");
			return false;
		}

		DataColumnStore* data = _dataStore.get();

		size_t index = 0, lenDataStore = data->getMaxSites();
		Sites::SiteData tmpSite;

		if (lenDataStore == 0) {
			_error_obj->setErrorMessage("Datastore has no entries");
			return false;
		}
		else {
			if (!data->getSiteData(0, &tmpSite)) {
				_error_obj->setErrorMessage("Failed to start Column Search initilisation");
				return false;
			}
			// Directly enter the initial batch
			_Latitiude.emplace_back(0, tmpSite.Latitude);

			_Longitude.emplace_back(0, tmpSite.Longitude);

			auto topit = data->TopographyDictionary.find(tmpSite.MapTopography);
			if (topit == data->TopographyDictionary.end()) {
				_error_obj->setErrorMessage("Topography not found in dictionary");
				return false;
			}
			else {
				_TopographyGroups.emplace_back(0, std::distance(data->TopographyDictionary.begin(), topit));
			}

			auto diffit = data->DifficultyDictionary.find(tmpSite.DifficultyChallenge);
			if (diffit == data->DifficultyDictionary.end()) {
				_error_obj->setErrorMessage("Difficulty not found in dictionary");
				return false;
			}
			else {
				_DififcultyGroups.emplace_back(0, std::distance(data->DifficultyDictionary.begin(), diffit));
			}

			_MetalsGroups.emplace_back(0, tmpSite.Resources[0]);
			_RareMetalsGroups.emplace_back(0, tmpSite.Resources[1]);
			_ConcreteGroups.emplace_back(0, tmpSite.Resources[2]);
			_WaterGroups.emplace_back(0, tmpSite.Resources[3]);

			_DustDevilGroups.emplace_back(0, tmpSite.Disasters[0]);
			_DustStormGroups.emplace_back(0, tmpSite.Disasters[1]);
			_MeteorGroups.emplace_back(0, tmpSite.Disasters[2]);
			_ColdWaveGroups.emplace_back(0, tmpSite.Disasters[3]);

			auto mapit = data->MapNameDictionary.find(tmpSite.MapName);
			if (mapit == data->MapNameDictionary.end()) {
				_error_obj->setErrorMessage("MapName not found in dictionary");
				return false;
			}
			else {
				_MapNameGroups.emplace_back(0, std::distance(data->MapNameDictionary.begin(), mapit));
			}

			auto locit = data->NamedLocationDictionary.find(tmpSite.MapLocation);
			if (locit == data->NamedLocationDictionary.end()) {
				_error_obj->setErrorMessage("MapLocation not found in dictionary");
				return false;
			}
			else {
				_NamedLocationGroups.emplace_back(0, std::distance(data->NamedLocationDictionary.begin(), locit));
			}

			if (tmpSite.Breakthroughs.bitset.none()) {
				_error_obj->setErrorMessage("Breakthroughs for site are empty");
				return false;
			}

			_BreakthroughGroups.emplace_back(0, tmpSite.Breakthroughs);
		}

		auto topologyIt = data->TopographyDictionary.end();
		auto difficultyIt = data->DifficultyDictionary.end();

		auto mapNameIt = data->MapNameDictionary.end();
		auto locationNameIt = data->NamedLocationDictionary.end();

		uint8_t tmpuint = 0;

		for (size_t i = 1; i < lenDataStore; i++)
		{
			// Get next site
			if (!data->getSiteData(i, &tmpSite)) {
				_error_obj->setErrorMessage("Failed to initialise all sites to Column Search");
				return false;
			}
			// ingest site to search groups:

			if (!handleColumnIntStretches(_Latitiude, i, tmpSite.Latitude)) {
				_error_obj->setErrorMessage("Failed to handle Latitude entry into column search");
				return false;
			}
			if (!handleColumnIntIncrementers(_Longitude, i, tmpSite.Longitude)) {
				_error_obj->setErrorMessage("Failed to handle Longitude entry into column search");
				return false;
			}

			// Topology/Difficulty
			topologyIt = data->TopographyDictionary.find(tmpSite.MapTopography);
			tmpuint = std::distance(data->TopographyDictionary.begin(), topologyIt);
			if (!handleColumnDictionaryGroups(_TopographyGroups, 2, i, tmpuint)) {
				_error_obj->setErrorMessage("Failed to handle Topography entry into column search");
				return false;
			}
			difficultyIt = data->DifficultyDictionary.find(tmpSite.DifficultyChallenge);
			tmpuint = std::distance(data->DifficultyDictionary.begin(), difficultyIt);
			if (!handleColumnDictionaryGroups(_DififcultyGroups, 2, i, tmpuint)) {
				_error_obj->setErrorMessage("Failed to handle DifficultyChallenge entry into column search");
				return false;
			}

			// Resources
			if (!handleColumnUintGroups(_MetalsGroups, i, tmpSite.Resources[0])) {
				_error_obj->setErrorMessage("Failed to handle Metals entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_RareMetalsGroups, i, tmpSite.Resources[1])) {
				_error_obj->setErrorMessage("Failed to handle RareMetals entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_ConcreteGroups, i, tmpSite.Resources[2])) {
				_error_obj->setErrorMessage("Failed to handle Concrete entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_WaterGroups, i, tmpSite.Resources[3])) {
				_error_obj->setErrorMessage("Failed to handle Water entry into column search");
				return false;
			}

			// Disasters
			if (!handleColumnUintGroups(_DustDevilGroups, i, tmpSite.Disasters[0])) {
				_error_obj->setErrorMessage("Failed to handle DustDevil entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_DustStormGroups, i, tmpSite.Disasters[1])) {
				_error_obj->setErrorMessage("Failed to handle DustStorm entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_MeteorGroups, i, tmpSite.Disasters[2])) {
				_error_obj->setErrorMessage("Failed to handle Meteor entry into column search");
				return false;
			}
			if (!handleColumnUintGroups(_ColdWaveGroups, i, tmpSite.Disasters[3])) {
				_error_obj->setErrorMessage("Failed to handle ColdWave entry into column search");
				return false;
			}

			// Map name/location
			mapNameIt = data->MapNameDictionary.find(tmpSite.MapName);
			tmpuint = std::distance(data->MapNameDictionary.begin(), mapNameIt);
			if (!handleColumnDictionaryGroups(_MapNameGroups, 6, i, tmpuint)) {
				_error_obj->setErrorMessage("Failed to handle Map Name entry into column search");
				return false;
			}
			locationNameIt = data->NamedLocationDictionary.find(tmpSite.MapLocation);
			tmpuint = std::distance(data->NamedLocationDictionary.begin(), locationNameIt);
			if (!handleColumnDictionaryGroups(_NamedLocationGroups, 4, i, tmpuint)) {
				_error_obj->setErrorMessage("Failed to handle Named Location entry into column search");
				return false;
			}

			// Breakthrough
			if (!handleBreakthroughGroups(_BreakthroughGroups, i, tmpSite.Breakthroughs)) {
				_error_obj->setErrorMessage("Failed to handle Breakthrough entry into column search");
				return false;
			}
		}

		// Collapse column groups

		collapseGroupings(_TopographyGroups);

		collapseGroupings(_DififcultyGroups);

		collapseGroupings(_MetalsGroups);
		collapseGroupings(_RareMetalsGroups);
		collapseGroupings(_ConcreteGroups);
		collapseGroupings(_WaterGroups);

		collapseGroupings(_DustDevilGroups);
		collapseGroupings(_DustStormGroups);
		collapseGroupings(_MeteorGroups);
		collapseGroupings(_ColdWaveGroups);

		collapseGroupings(_MapNameGroups);
		collapseGroupings(_NamedLocationGroups);

		collapseBreakthroughGroups(_BreakthroughGroups);

		initialised = true;
		return true;
	}
};