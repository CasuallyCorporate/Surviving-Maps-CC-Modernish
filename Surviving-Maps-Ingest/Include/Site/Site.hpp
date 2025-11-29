#pragma once

#include <map>
#include <vector>
#include <array>

#include "IErrors.hpp"

#include "Headers.hpp"
#include "Breakthroughs.hpp"

namespace Sites {

	struct SiteData {
		char LatNS;
		int Latitude;
		
		char LongEW;
		int Longitude;

		std::array<uint8_t, 4> Resources;
		std::array<uint8_t, 4> Disasters;

		std::string MapName;
		std::string MapLocation;
		std::string MapTopography;

		int DifficultyChallenge;
		int Altitude;
		int Temperature;

		Breakthroughs::btrData Breakthroughs;
	};

	class Site {
	public:
		//Defaults
		static const char _defaultchar = 'E';

		SiteData data = {
			_defaultchar, -1,
			
			_defaultchar, -1,

			{UINT8_MAX,UINT8_MAX,UINT8_MAX,UINT8_MAX},
			{UINT8_MAX,UINT8_MAX,UINT8_MAX,UINT8_MAX},

			"",
			"",
			"",

			-1,
			INT_MIN,
			INT_MIN,

			Breakthroughs::btrData {}
		};

		// Breakthroughs
		std::map<Header::Headers, Breakthroughs::breakthrough_Enum>* _breakthroughs = nullptr;
		std::array<Breakthroughs::breakthrough_Enum, 13> _cachedBreakthroughs;
		Breakthroughs::btrData _returnBreakthroughs;


		Site() {}
		Site(const Site& other) {

			data = other.data;

			_cachedBreakthroughs = other._cachedBreakthroughs;
			// _cachedBreakthroughs should refresh on it's own
			// No defaults
		}

		void transferContentsFrom(const Site& other) {
#pragma warning( disable : 26478)
			data = std::move(other.data);

			_cachedBreakthroughs = std::move(other._cachedBreakthroughs);
			// _cachedBreakthroughs should refresh on it's own
			// No defaults
#pragma warning(pop)
		}
	};

	// Coordinates
#pragma region Coordinates
	static bool setLatitudeNS(char NS, Site* site) {
		if (site->data.LatNS != site->_defaultchar)
		{
			return false;
		}
		else {
			site->data.LatNS = NS;
			return true;
		}
	}
	static bool getLatitiudeNS(char* NS, Site* site) {
		if (site->data.LatNS != site->_defaultchar)
		{
			*NS = site->data.LatNS;
			return true;
		}
		else {
			return false;
		}
	}
	static bool setLongitudeEW(char EW, Site* site) {
		if (site->data.LongEW != site->_defaultchar)
		{
			return false;
		}
		else {
			site->data.LongEW = EW;
			return true;
		}
	}
	static bool getLongitudeEW(char* EW, Site* site) {
		if (site->data.LongEW != site->_defaultchar)
		{
			*EW = site->data.LongEW;
			return true;
		}
		else {
			return false;
		}
	}
	static bool setLatitudeValue(int value, Site* site) {
		if (site->data.Latitude < 0)
		{
			site->data.Latitude = value;
			return true;
		}
		else {
			return false;
		}
	}
	static bool setLongitudeValue(int value, Site* site) {
		if (site->data.Longitude < 0)
		{
			site->data.Longitude = value;
			return true;
		}
		else {
			return false;
		}
	}
	static bool getLatitudeValue(int* value, Site* site) {
		if (site->data.Latitude < 0) {
			return false;
		}
		else {
			*value = site->data.Latitude;
			return true;
		}
	}
	static bool getLongitudeValue(int* value, Site* site) {
		if (site->data.Longitude < 0) {
			return false;
		}
		else {
			*value = site->data.Longitude;
			return true;
		}
	}
#pragma endregion

	// Resources
#pragma region Resources
	static bool setResourceValue(Header::Headers resourceNum, uint8_t value, Site* site) {
		if (resourceNum < Header::Headers::Metals || resourceNum > Header::Headers::Water)
		{
			return false;
		}

		uint8_t resourceindex = resourceNum - Header::Headers::Metals;

		if (site->data.Resources[resourceindex] > 4) {
			// Set
			site->data.Resources[resourceindex] = value;
			return true;
		}
		return false;
	}
	static bool getResourceValue(Header::Headers resourceNum, uint8_t* value, Site* site) {
		if (resourceNum < Header::Headers::Metals || resourceNum > Header::Headers::Water)
		{
			return false;
		}

		uint8_t resourceindex = resourceNum - Header::Headers::Metals;

		if (site->data.Resources[resourceindex] > 4) {
			return false;
		}
		*value = site->data.Resources[resourceindex];
		return true;
	}
#pragma endregion

	// Disasters
#pragma region Disasters
	static bool setDisasterValue(Header::Headers disasternum, uint8_t value, Site* site) {
		if (disasternum < Header::Headers::DustDevils || disasternum > Header::Headers::ColdWaves)
		{
			return false;
		}

		uint8_t disasterIndex = disasternum - Header::Headers::DustDevils;

		if (site->data.Disasters[disasterIndex] > 4) {
			// Set
			site->data.Disasters[disasterIndex] = value;
			return true;
		}
		return false;
	}
	static bool getDisasterValue(Header::Headers disasternum, uint8_t* value, Site* site) {
		if (disasternum < Header::Headers::DustDevils || disasternum > Header::Headers::ColdWaves)
		{
			return false;
		}

		uint8_t disasterIndex = disasternum - Header::Headers::DustDevils;

		if (site->data.Disasters[disasterIndex] > 4) {
			return false;
		}
		*value = site->data.Disasters[disasterIndex];
		return true;
	}
#pragma endregion

	// Map details
#pragma region MapDetails
	static bool setMapName(std::string name, Site* site) {
		if (site->data.MapName == "")
		{
			site->data.MapName = name;
			return true;
		}
		else {
			return false;
		}
	}
	static bool getMapName(std::string* name, Site* site) {
		*name = site->data.MapName;

		return true;
	}
	static bool setMapLocation(std::string location, Site* site) {
		if (site->data.MapLocation == "")
		{
			std::replace(location.begin(), location.end(), ' ', '_'); // change space to _
			std::transform(location.begin(), location.end(), location.begin(), ::toupper);
			site->data.MapLocation = location;
			return true;
		}
		else {
			return false;
		}
	}
	static bool getMapLocation(std::string* location, Site* site) {
		*location = site->data.MapLocation;
		return true;
	}
	static bool setMapTopography(std::string topography, Site* site) {
		if (site->data.MapTopography == "") {
			std::replace(topography.begin(), topography.end(), ' ', '_'); // change space to _
			std::transform(topography.begin(), topography.end(), topography.begin(), ::toupper);
			site->data.MapTopography = topography;
			return true;
		}
		else {
			return false;
		}
	}
	static bool getMapTopography(std::string* topography, Site* site) {
		*topography = site->data.MapTopography;
		return true;
	}
	static bool setMapDifficulty(int difficulty, Site* site) {
		if (site->data.DifficultyChallenge < 0) {
			site->data.DifficultyChallenge = difficulty;
			return true;
		}
		else {
			return false;
		}
	}
	static bool getMapDifficulty(int* difficulty, Site* site) {
		if (site->data.DifficultyChallenge < 0)
		{
			return false;
		}
		else {
			*difficulty = site->data.DifficultyChallenge;
			return true;
		}
	}
	static bool setMapAltitude(int altitude, Site* site) {
		if (site->data.Altitude != INT_MIN) {
			return false;
		}
		else {
			site->data.Altitude = altitude;
			return true;
		}
	}
	static bool getMapAltitude(int* altitude, Site* site) {
		if (site->data.Altitude != INT_MIN) {
			*altitude = site->data.Altitude;
			return true;
		}
		else {
			return false;
		}
	}
	static bool setMapTemperature(int temperature, Site* site) {
		if (site->data.Temperature != INT_MIN) {
			return false;
		}
		else {
			site->data.Temperature = temperature;
			return true;
		}
	}
	static bool getMapTemperatuire(int* temperature, Site* site) {
		if (site->data.Temperature != INT_MIN) {
			*temperature = site->data.Temperature;
			return true;
		}
		else {
			return false;
		}
	}
#pragma endregion

	// Breakthroughs
#pragma region Breakthroughs
	static bool setBreakthrough(Header::Headers BTRnumber, Breakthroughs::breakthrough_Enum type, Site* site) {
		if (BTRnumber < Header::Headers::Breakthrough1 || BTRnumber > Header::Headers::Breakthrough13)
		{
			return false;
		}
		if (site->_breakthroughs == nullptr) {
			site->_breakthroughs = new std::map<Header::Headers, Breakthroughs::breakthrough_Enum>();
		}

		std::map<Header::Headers, Breakthroughs::breakthrough_Enum>::iterator btrIt;
		btrIt = site->_breakthroughs->find(BTRnumber);
		if (btrIt == site->_breakthroughs->end())
		{
			site->_breakthroughs->insert_or_assign(BTRnumber, type);
			return true;
		}
		else {
			return false;
		}
	}

	static bool finaliseBreakthroughs(Site* site) {
		std::map<Header::Headers, Breakthroughs::breakthrough_Enum>::iterator btrIt;

		int cachedIndex = 0;
		for (size_t i = Header::Headers::Breakthrough1; i < Header::Headers::End; i++)
		{
			btrIt = site->_breakthroughs->find((Header::Headers)i);
			if (btrIt != site->_breakthroughs->end())
			{
				site->_cachedBreakthroughs[cachedIndex] = btrIt->second;
				cachedIndex++;
			}
		}
		delete site->_breakthroughs;
		site->_breakthroughs = nullptr;
		return true;
	}

	static std::array<Breakthroughs::breakthrough_Enum, 13>* getAllBreakthroughs(Site* site) {
		return &site->_cachedBreakthroughs;
	}
#pragma endregion
}