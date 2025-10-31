#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>

#include <fstream>
#include <sstream>

#include <iostream>

#include "Breakthroughs.hpp"
#include "Site/Site.hpp"

#include "IErrors.hpp"

#include "VariantData/DataColumnStore.hpp"

class CSVIngest {
private:
	std::string _csvURL;
	IErrors* _errors;

	void csvLineToVector(std::string* str, std::vector<std::string>* vect) {
		vect->clear();
		int begin = 0, len = 0, endStr = str->size() - 1;
		for (size_t i = 0; i <= endStr; i++) {
			if (str->at(i) == ',') {
				if (i == begin) {
					// csv entry empty
					vect->emplace_back("");
				}
				else {
					std::string_view _lineitem = std::string_view(str->data() + begin, i - begin);
					if (_lineitem.ends_with(' ')) {
						// Need to remove any trailing spaces
						int less = 1;
						while (_lineitem.ends_with(' '))
						{
							_lineitem = std::string_view(str->data() + begin, i - (begin + less));
							less++;
						}
					}
					vect->emplace_back(_lineitem);
				}
				begin = i + 1;
			}
		}
		if (vect->size() > 0)
		{ // Add the last 
			std::string_view _lineitem = std::string_view(str->data() + begin, (endStr + 1) - begin);
			if (_lineitem.ends_with(' ')) {
				// Need to remove any trailing spaces
				int less = 1;
				while (_lineitem.ends_with(' '))
				{
					_lineitem = std::string_view(str->data() + begin, (endStr + 1) - (begin + less));
					less++;
				}
			}
			vect->emplace_back(_lineitem);
		}
	}
	bool stringToInt(std::string* string, int* num) {
		try
		{
			*num = std::atoi(string->c_str());
			return true;
		}
		catch (const std::exception&)
		{
			_errors->setErrorMessage("StringToInt was not correctly parsed");
			return false;
		}
	}
	bool stringToChar(std::string* string, char* singleChar) {
		try
		{
			*singleChar = string->at(0);
			return true;
		}
		catch (const std::exception&)
		{
			_errors->setErrorMessage("StringToChar was not correctly parsed");
			return false;
		}
	}
public:
	CSVIngest(std::string csvURL, IErrors* error_obj) {
		_csvURL = csvURL;
		_errors = error_obj;
	}

	bool populateData(DataColumnStore* datFlat) {
		std::ifstream csvFile(_csvURL);
		std::string line;
		// First line is csv header
		try
		{
			std::getline(csvFile, line);
		}
		catch (const std::exception&)
		{
			_errors->setErrorMessage("Could not read csvURL");
			return false;
		}

		std::vector<std::string> lineItems;
		csvLineToVector(&line, &lineItems);

		// Parse header to column map
		std::map<Header::Headers, int> csvHeaderColumn;
		int itemIndex = 0;
		for (std::string column : lineItems) {

			if (auto search = Header::csvToHeader.find(column); search != Header::csvToHeader.end()) {
				csvHeaderColumn[search->second] = itemIndex;
			}
			itemIndex++;
		}

		std::vector<Header::Headers> requiredHeaders = Header::minRequiredHeaders;
		std::map<Header::Headers, int>::iterator mapIt;
		for (mapIt = csvHeaderColumn.begin(); mapIt != csvHeaderColumn.end(); mapIt++) {
			std::vector<Header::Headers>::iterator reqIt = std::find(requiredHeaders.begin(), requiredHeaders.end(), mapIt->first);

			if (reqIt == requiredHeaders.end())
			{
				continue;
			}
			else {
				requiredHeaders.erase(reqIt);
			}
		}
		if (requiredHeaders.size() != 0)
		{
			_errors->setErrorMessage("Minimum headers do not exist");
			return false;
		}

		// find how many entries we have
		size_t totalEntries = 0;
		while (std::getline(csvFile, line)) {
			totalEntries++;
		}

		// set the number of entires for the column data
		if (!datFlat->setCollectionMax(totalEntries)) {
			_errors->setErrorMessage("Could not allocate flat dat entry size");
			return false;
		}

		// reset and get back to the start of the entries
		csvFile.clear();
		csvFile.seekg(0);
		std::getline(csvFile, line);

		// Site thingy
		Sites::Site tmpSite;

		// Deal with the rest of the csv, into the tempSite
		int recNum = 0;
		int tmpNum;
		char tmpChar;
		while (std::getline(csvFile, line)) {
			csvLineToVector(&line, &lineItems);
			tmpSite.transferContentsFrom(Sites::Site());

			for (mapIt = csvHeaderColumn.begin(); mapIt != csvHeaderColumn.end(); mapIt++)
			{
				switch (mapIt->first)
				{
#pragma region LatitudeDeg
				case Header::Headers::LatitudeDeg:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setLatitudeValue(tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("LatitudeDeg column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region LatitudeNS
				case Header::Headers::LatitudeNS:
					// get char from std::string
					if (lineItems.at(mapIt->second).size() == 1)
					{
						if (stringToChar(&lineItems.at(mapIt->second), &tmpChar)) {
							Sites::setLatitudeNS(tmpChar, &tmpSite);
							break;
						}
					}
					_errors->setErrorMessage("LatitudeNS column was not correctly parsed");
					return false;
					break;
#pragma endregion
#pragma region LongitudeDeg
				case Header::Headers::LongitudeDeg:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setLongitudeValue(tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("LongitudeDeg column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region LongitudeEW
				case Header::Headers::LongitudeEW:
					// get char from std::string
					if (lineItems.at(mapIt->second).size() == 1)
					{
						if (stringToChar(&lineItems.at(mapIt->second), &tmpChar)) {
							Sites::setLongitudeEW(tmpChar, &tmpSite);
							break;
						}
					}
					_errors->setErrorMessage("LongitudeEW column was not correctly parsed");
					return false;
					break;
#pragma endregion
#pragma region Resources_MetalsRareMetalsConcreteWater
				case Header::Headers::Metals:
				case Header::Headers::RareMetals:
				case Header::Headers::Concrete:
				case Header::Headers::Water:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setResourceValue(mapIt->first, tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("Resource column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region Disasters_DustDevilsDustStormsMeteorsColdWaves
				case Header::Headers::DustDevils:
				case Header::Headers::DustStorms:
				case Header::Headers::Meteors:
				case Header::Headers::ColdWaves:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setDisasterValue(mapIt->first, tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("Disaster column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region MapName
				case Header::Headers::MapName:
					Sites::setMapName(lineItems.at(mapIt->second), &tmpSite);
					break;
					//,	Topography,
#pragma endregion
#pragma region NamedLocation
				case Header::Headers::NamedLocation:
					Sites::setMapLocation(lineItems.at(mapIt->second), &tmpSite);
					break;
#pragma endregion
#pragma region Topography
				case Header::Headers::Topography:
					Sites::setMapTopography(lineItems.at(mapIt->second), &tmpSite);
					break;
#pragma endregion
#pragma region DifficultyChallenge
				case Header::Headers::DifficultyChallenge:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setMapDifficulty(tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("DifficultyChallenge column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region Altitude
				case Header::Headers::Altitude:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setMapAltitude(tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("Altitude column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region Temperature
				case Header::Headers::Temperature:
					// get int from std::string
					try
					{
						if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
							Sites::setMapTemperature(tmpNum, &tmpSite);
						}
						else {
							return false;
						}
					}
					catch (const std::exception&)
					{
						_errors->setErrorMessage("Altitude column was not correctly parsed");
						return false;
					}
					break;
#pragma endregion
#pragma region Breakthroughs
				case Header::Headers::Breakthrough1:
				case Header::Headers::Breakthrough2:
				case Header::Headers::Breakthrough3:
				case Header::Headers::Breakthrough4:
				case Header::Headers::Breakthrough5:
				case Header::Headers::Breakthrough6:
				case Header::Headers::Breakthrough7:
				case Header::Headers::Breakthrough8:
				case Header::Headers::Breakthrough9:
				case Header::Headers::Breakthrough10:
				case Header::Headers::Breakthrough11:
				case Header::Headers::Breakthrough12:
				case Header::Headers::Breakthrough13:
				{
					// Translate text to breakthrough
					auto btrIt = Breakthroughs::stringToEnum.find(lineItems.at(mapIt->second));

					if (btrIt == Breakthroughs::stringToEnum.end())
					{
						_errors->setErrorMessage("Breakthrough column text is not a valid breakthrough");
						return false;
					}
					else {
						// Add breakthrough
						if (!Sites::setBreakthrough(mapIt->first, btrIt->second, &tmpSite)) {
							return false;
						}
					}
				}
				break;
#pragma endregion
				default:
					// Do nothing
					break;
				}
			}
			recNum++;

			// Finalise breakthroughs for site (deleting the map)
			Sites::finaliseBreakthroughs(&tmpSite);

			// Succesfully parsed. Add to Stores
			if (!datFlat->addSite(&tmpSite)) {
				_errors->setErrorMessage("Could not add site");
				return false;
			}
		}

		return datFlat->setEndOfData();
	}
};