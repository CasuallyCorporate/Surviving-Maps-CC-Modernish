#pragma once

#include <map>
#include <vector>

#include "IErrors.hpp"

#include "Breakthroughs.hpp"

namespace Sites {
	const enum Header {
		None = 0,
		LatitudeDeg, LatitudeNS, LongitudeDeg, LongitudeEW,

		Metals, RareMetals, Concrete, Water,

		DustDevils, DustStorms, Meteors, ColdWaves,

		MapName, NamedLocation,	Topography,
		DifficultyChallenge, Altitude, Temperature,

		Breakthrough1, Breakthrough2, Breakthrough3, Breakthrough4, Breakthrough5, Breakthrough6, Breakthrough7, Breakthrough8, Breakthrough9, Breakthrough10,
		Breakthrough11, Breakthrough12, Breakthrough13,
		End
	};

	const std::map<std::string, Header> stringToHeader{
		{"Latitude °", Header::LatitudeDeg}, {"Latitude", Header::LatitudeNS}, {"Longitude °", Header::LongitudeDeg}, {"Longitude", Header::LongitudeEW},

		{"Metals", Header::Metals}, {"Rare Metals", Header::RareMetals}, {"Concrete", Header::Concrete}, {"Water", Header::Water},

		{"Dust Devils", Header::DustDevils}, {"Dust Storms", Header::DustStorms}, {"Meteors", Header::Meteors}, {"Cold Waves", Header::ColdWaves},

		{"Map Name", Header::MapName}, {"Named Location", Header::NamedLocation},
		{"Topography", Header::Topography}, {"Difficulty Challenge", Header::DifficultyChallenge}, {"Altitude", Header::Altitude}, {"Temperature", Header::Temperature},

		{"Breakthrough 1", Header::Breakthrough1}, {"Breakthrough 2", Header::Breakthrough2}, {"Breakthrough 3", Header::Breakthrough3}, {"Breakthrough 4", Header::Breakthrough4},
		{"Breakthrough 5", Header::Breakthrough5}, {"Breakthrough 6", Header::Breakthrough6}, {"Breakthrough 7", Header::Breakthrough7}, {"Breakthrough 8", Header::Breakthrough8},
		{"Breakthrough 9", Header::Breakthrough9}, {"Breakthrough 10", Header::Breakthrough10}, {"Breakthrough 11", Header::Breakthrough11}, {"Breakthrough 12", Header::Breakthrough12},
		{"Breakthrough 13", Header::Breakthrough13}
	};

	const std::map<Header, std::string> headerToEnum{
	{Header::LatitudeDeg, "Latitude °" }, {Header::LatitudeNS, "Latitude"}, {Header::LongitudeDeg, "Longitude °"}, {Header::LongitudeEW, "Longitude"},

	{Header::Metals, "Metals"}, {Header::RareMetals, "Rare Metals"}, {Header::Concrete, "Concrete"}, {Header::Water, "Water"},

	{Header::DustDevils, "Dust Devils"}, {Header::DustStorms, "Dust Storms"}, {Header::Meteors, "Meteors"}, {Header::ColdWaves, "Cold Waves"},

	{Header::MapName, "Map Name"}, {Header::NamedLocation, "Named Location"},
	{Header::Topography, "Topography"}, {Header::DifficultyChallenge, "Difficulty Challenge"}, {Header::Altitude, "Altitude"}, {Header::Temperature, "Temperature"},

	{Header::Breakthrough1, "Breakthrough 1"}, {Header::Breakthrough2, "Breakthrough 2"}, {Header::Breakthrough3, "Breakthrough 3"}, {Header::Breakthrough4, "Breakthrough 4"},
	{Header::Breakthrough5, "Breakthrough 5"}, {Header::Breakthrough6, "Breakthrough 6"}, {Header::Breakthrough7, "Breakthrough 7"}, {Header::Breakthrough8, "Breakthrough 8"},
	{Header::Breakthrough9, "Breakthrough 9"}, {Header::Breakthrough10, "Breakthrough 10"}, {Header::Breakthrough11, "Breakthrough 11"}, {Header::Breakthrough12, "Breakthrough 12"},
	{Header::Breakthrough13, "Breakthrough 13"}
	};

	const std::vector<Sites::Header> minRequiredHeaders {
		LatitudeDeg, LatitudeNS, LongitudeDeg, LongitudeEW,
		Metals, RareMetals, Concrete, Water,
		DustDevils, DustStorms, Meteors, ColdWaves,
		
		Breakthrough1, Breakthrough2, Breakthrough3, Breakthrough4, Breakthrough5, Breakthrough6, Breakthrough7, Breakthrough8, Breakthrough9, Breakthrough10,
		Breakthrough11, Breakthrough12, Breakthrough13,
	};
}

class Site: IErrors {
private:
	// Coordinates
	char _defaultchar = 'E';
	char _latNS = _defaultchar, _longEW = _defaultchar;
	int _latitude = -1, _longitude = -1;

	// Resources
	std::map<Sites::Header, int> _resources;

	// Disasters;
	std::map<Sites::Header, int> _disasters;

	// Map details
	std::string _defaultString = "";
	std::string _mapName = _defaultString, _mapLocation = _defaultString,
		_mapTopography = _defaultString;
	int _mapDifficultyChallenge = -1, _mapAltitude = INT_MIN, _mapTemperature = INT_MIN;

	// Breakthroughs
	std::map<Sites::Header, Breakthroughs::breakthrough_Enum> _breakthroughs;
	std::vector<Breakthroughs::breakthrough_Enum> _cachedBreakthroughs;

public:
	Site() {}
	Site(const Site& other) {
		_latNS = other._latNS; _longEW = other._longEW;
		_latitude = _latitude; _longitude = _longitude;

		_resources = other._resources;

		_disasters = other._disasters;

		_mapName = other._mapName; _mapLocation = other._mapLocation;
		_mapTopography = other._mapTopography;
		_mapDifficultyChallenge = other._mapDifficultyChallenge; _mapAltitude = other._mapAltitude;
		_mapTemperature = other._mapTemperature;

		_breakthroughs = other._breakthroughs;
		// _cachedBreakthroughs should refresh on it's own
		// No defaults
	}

	void transferContentsFrom(const Site& other) {
		#pragma warning( disable : 26478)
		_latNS = std::move(other._latNS); _longEW = std::move(other._longEW);
		_latitude = std::move(_latitude); _longitude = std::move(_longitude);

		_resources = std::move(other._resources);

		_disasters = std::move(other._disasters);

		_mapName = std::move(other._mapName); _mapLocation = std::move(other._mapLocation);
		_mapTopography = std::move(other._mapTopography);
		_mapDifficultyChallenge = std::move(other._mapDifficultyChallenge); _mapAltitude = std::move(other._mapAltitude);
		_mapTemperature = std::move(other._mapTemperature);

		_breakthroughs = std::move(other._breakthroughs);
		// _cachedBreakthroughs should refresh on it's own
		// No defaults
		#pragma warning(pop)
	}

	// Coordinates
#pragma region Coordinates
	bool setLatitudeNS(char NS) {
		if (_latNS != _defaultchar)
		{
			setErrorMessage( { "Latitude NS already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
		else {
			_latNS = NS;
			return true;
		}
	}
	bool getLatitiudeNS(char* NS) {
		if (_latNS != _defaultchar)
		{
			*NS = _latNS;
			return true;
		}
		else {
			setErrorMessage( { "Latitude NS not set", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setLongitudeEW(char EW) {
		if (_longEW != _defaultchar)
		{
			setErrorMessage( { "Latitude EW already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
		else {
			_longEW = EW;
			return true;
		}
	}
	bool getLongitudeEW(char* EW) {
		if (_longEW != _defaultchar)
		{
			*EW = _longEW;
			return true;
		}
		else {
			setErrorMessage( { "Latitude EW not set", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setLatitudeValue(int value) {
		if (_latitude < 0)
		{
			_latitude = value;
			return true;
		}
		else {
			setErrorMessage( { "Latitude already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setLongitudeValue(int value) {
		if (_longitude < 0)
		{
			_longitude = value;
			return true;
		}
		else {
			setErrorMessage( { "Longitude already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getLatitudeValue(int* value) {
		if (_latitude < 0) {
			setErrorMessage( { "Latitude not set", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
		else {
			*value = _latitude;
			return true;
		}
	}
	bool getLongitudeValue(int* value) {
		if (_longitude < 0) {
			setErrorMessage( { "Longitude not set", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
		else {
			*value = _longitude;
			return true;
		}
	}
#pragma endregion

	// Resources
#pragma region Resources
	bool setResourceValue(Sites::Header resourceNum, int value) {
		if (resourceNum < Sites::Header::Metals || resourceNum > Sites::Header::Water)
		{
			setErrorMessage( { "Given Header is not a resource header", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}

		std::map<Sites::Header, int>::iterator It;
		It = _resources.find(resourceNum);
		if (It == _resources.end())
		{
			_resources[resourceNum] = value;
			return true;
		}
		else {
			setErrorMessage( { "Resource already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getResourceValue(Sites::Header resourceNum, int* value) {
		if (resourceNum < Sites::Header::Metals || resourceNum > Sites::Header::Water)
		{
			setErrorMessage( { "Given Header is not a resource header", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}

		std::map<Sites::Header, int>::iterator It;
		It = _resources.find(resourceNum);
		if (It == _resources.end())
		{
			setErrorMessage( { "Resource header does not exist in resources", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
		else {
			*value = It->second;
			return true;
		}
	}
#pragma endregion

	// Disasters
#pragma region Disasters
	bool setDisasterValue(Sites::Header disasternum, int value) {
		if (disasternum < Sites::Header::DustDevils || disasternum > Sites::Header::ColdWaves)
		{
			setErrorMessage( { "Given Header is not a disaster header", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}

		std::map<Sites::Header, int>::iterator It;
		It = _disasters.find(disasternum);
		if (It == _disasters.end())
		{
			_disasters[disasternum] = value;
			return true;
		}
		else {
			setErrorMessage( { "Disaster already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getDisasterValue(Sites::Header disasternum, int* value) {
		if (disasternum < Sites::Header::DustDevils || disasternum > Sites::Header::ColdWaves)
		{
			setErrorMessage( { "Given Header is not a disaster header", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}

		std::map<Sites::Header, int>::iterator It;
		It = _disasters.find(disasternum);
		if (It == _disasters.end())
		{
			setErrorMessage( { "Disaster header does not exist in Disasters", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
		else {
			*value = It->second;
			return true;
		}
	}
#pragma endregion

	// Map details
#pragma region MapDetails
	bool setMapName(std::string name) {
		if (_mapName == _defaultString)
		{
			_mapName = name;
			return true;
		}
		else {
			setErrorMessage( { "Map name already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getMapName(std::string* name) {
		if (_mapName != _defaultString)
		{
			*name = _mapName;
			return true;
		}
		else {
			setErrorMessage( { "Map name does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setMapLocation(std::string location) {
		if (_mapLocation == _defaultString)
		{
			_mapLocation = location;
			return true;
		}
		else {
			setErrorMessage( { "Map location already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getMapLocation(std::string* location) {
		if (_mapLocation != _defaultString) {
			*location = _mapLocation;
			return true;
		}
		else {
			setErrorMessage( { "Map location does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setMapTopography(std::string topography) {
		if (_mapTopography == _defaultString) {
			_mapTopography = topography;
			return true;
		}
		else {
			setErrorMessage( { "Map topography already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getMapTopography(std::string* topography) {
		if (_mapTopography != _defaultString) {
			*topography = _mapTopography;
			return true;
		}
		else {
			setErrorMessage( { "Map topography does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setMapDifficulty(int difficulty) {
		if (_mapDifficultyChallenge < 0) {
			_mapDifficultyChallenge = difficulty;
			return true;
		}
		else {
			setErrorMessage( { "Map difficulty already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getMapDifficulty(int* difficulty) {
		if (_mapDifficultyChallenge < 0)
		{
			setErrorMessage( { "Map difficulty does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
		else {
			*difficulty = _mapDifficultyChallenge;
			return true;
		}
	}
	bool setMapAltitude(int altitude) {
		if (_mapAltitude != INT_MIN) {
			setErrorMessage( { "Map altitude already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
		else {
			_mapAltitude = altitude;
			return true;
		}
	}
	bool getMapAltitude(int* altitude) {
		if (_mapAltitude != INT_MIN) {
			*altitude = _mapAltitude;
			return true;
		}
		else {
			setErrorMessage( { "Map topography does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
	bool setMapTemperature(int temperature) {
		if (_mapTemperature != INT_MIN) {
			setErrorMessage( { "Map temperature already exists", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
		else {
			_mapTemperature = temperature;
			return true;
		}
	}
	bool getMapTemperatuire(int* temperature) {
		if (_mapTemperature != INT_MIN) {
			*temperature = _mapTemperature;
			return true;
		}
		else {
			setErrorMessage( { "Map temperature does not exist", Errors::ToFrom::Read, Errors::ToFrom::None } );
			return false;
		}
	}
#pragma endregion

	// Breakthroughs
#pragma region Breakthroughs
	bool setBreakthrough(Sites::Header BTRnumber, Breakthroughs::breakthrough_Enum type) {
		if (BTRnumber < Sites::Header::Breakthrough1 || BTRnumber > Sites::Header::Breakthrough13)
		{
			setErrorMessage( { "Given Header is not a Breakthrough header", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}

		std::map<Sites::Header, Breakthroughs::breakthrough_Enum>::iterator btrIt;
		btrIt = _breakthroughs.find(BTRnumber);
		if (btrIt == _breakthroughs.end())
		{
			_breakthroughs[BTRnumber] = type;
			return true;
		}
		else {
			setErrorMessage( { "Breakthroug already set", Errors::ToFrom::Create, Errors::ToFrom::None } );
			return false;
		}
	}
	bool getBreakthrough(Sites::Header number, Breakthroughs::breakthrough_Enum* type) {
		if (number < Sites::Header::Breakthrough1 || number > Sites::Header::Breakthrough13)
		{
			setErrorMessage( {"Given Header is not a Breakthrough Number", Errors::ToFrom::Create, Errors::ToFrom::None} );
			return false;
		}

		std::map<Sites::Header, Breakthroughs::breakthrough_Enum>::iterator btrIt;
		btrIt = _breakthroughs.find(number);
		if (btrIt == _breakthroughs.end())
		{
			setErrorMessage( {"BreakthroughNumber does not exist in Breakthroughs", Errors::ToFrom::Read, Errors::ToFrom::None} );
			return false;
		}
		else {
			*type = btrIt->second;
			return true;
		}
	}
	std::vector<Breakthroughs::breakthrough_Enum> getAllBreakthroughs() {
		if (_cachedBreakthroughs.size() > 0)
		{
			return _cachedBreakthroughs;
		}

		std::map<Sites::Header, Breakthroughs::breakthrough_Enum>::iterator btrIt;

		for (size_t i = Sites::Header::Breakthrough1; i < Sites::Header::End; i++)
		{
			btrIt = _breakthroughs.find((Sites::Header)i);
			if (btrIt != _breakthroughs.end())
			{
				_cachedBreakthroughs.push_back(btrIt->second);
			}
		}
		return _cachedBreakthroughs;
	}
#pragma endregion
	
	// IError
// IErrored publics
	Errors::ErrorMessage getErrorPublic() override {
		return getErrorMessage();
	}
	bool hasErroredPublic() override {
		return hasErrored();
	}

	friend Site;
};