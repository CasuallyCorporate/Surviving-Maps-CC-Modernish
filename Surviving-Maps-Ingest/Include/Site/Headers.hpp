#include <map>
#include <set>
#include <string>
#include <vector>

#pragma once

namespace Header {
	const enum Headers : uint8_t {
		LatitudeDeg, LatitudeNS, LongitudeDeg, LongitudeEW,

		Metals, RareMetals, Concrete, Water,

		DustDevils, DustStorms, Meteors, ColdWaves,

		MapName, NamedLocation, Topography,
		DifficultyChallenge, Altitude, Temperature,

		Breakthrough1, Breakthrough2, Breakthrough3, Breakthrough4, Breakthrough5, Breakthrough6, Breakthrough7, Breakthrough8, Breakthrough9, Breakthrough10,
		Breakthrough11, Breakthrough12, Breakthrough13,
		End
	};

	static const std::set<Headers> HeaderCanCompress{
		Headers::LatitudeNS, Headers::LongitudeEW,

		Headers::MapName, Headers::NamedLocation, Headers::Topography,
		Headers::DifficultyChallenge,

		Headers::Breakthrough1, Headers::Breakthrough2, Headers::Breakthrough3, Headers::Breakthrough4, Headers::Breakthrough5,
		Headers::Breakthrough6, Headers::Breakthrough7, Headers::Breakthrough8,	Headers::Breakthrough9, Headers::Breakthrough10,
		Headers::Breakthrough11, Headers::Breakthrough12, Headers::Breakthrough13
	};

	static const std::map<std::string, Headers> csvToHeader{
		{"Latitude Â°", Headers::LatitudeDeg}, {"Latitude", Headers::LatitudeNS}, {"Longitude Â°", Headers::LongitudeDeg}, {"Longitude", Headers::LongitudeEW},

		{"Metals", Headers::Metals}, {"Rare Metals", Headers::RareMetals}, {"Concrete", Headers::Concrete}, {"Water", Headers::Water},

		{"Dust Devils", Headers::DustDevils}, {"Dust Storms", Headers::DustStorms}, {"Meteors", Headers::Meteors}, {"Cold Waves", Headers::ColdWaves},

		{"Map Name", Headers::MapName}, {"Named Location", Headers::NamedLocation},
		{"Topography", Headers::Topography}, {"Difficulty Challenge", Headers::DifficultyChallenge}, {"Altitude", Headers::Altitude}, {"Temperature", Headers::Temperature},

		{"Breakthrough 1", Headers::Breakthrough1}, {"Breakthrough 2", Headers::Breakthrough2}, {"Breakthrough 3", Headers::Breakthrough3}, {"Breakthrough 4", Headers::Breakthrough4},
		{"Breakthrough 5", Headers::Breakthrough5}, {"Breakthrough 6", Headers::Breakthrough6}, {"Breakthrough 7", Headers::Breakthrough7}, {"Breakthrough 8", Headers::Breakthrough8},
		{"Breakthrough 9", Headers::Breakthrough9}, {"Breakthrough 10", Headers::Breakthrough10}, {"Breakthrough 11", Headers::Breakthrough11}, {"Breakthrough 12", Headers::Breakthrough12},
		{"Breakthrough 13", Headers::Breakthrough13}
	};

	static const std::map<Headers, std::string> HeaderToString{
	{Headers::LatitudeDeg, "Latitude °" }, {Headers::LatitudeNS, "Latitude"}, {Headers::LongitudeDeg, "Longitude °"}, {Headers::LongitudeEW, "Longitude"},

	{Headers::Metals, "Metals"}, {Headers::RareMetals, "Rare Metals"}, {Headers::Concrete, "Concrete"}, {Headers::Water, "Water"},

	{Headers::DustDevils, "Dust Devils"}, {Headers::DustStorms, "Dust Storms"}, {Headers::Meteors, "Meteors"}, {Headers::ColdWaves, "Cold Waves"},

	{Headers::MapName, "Map Name"}, {Headers::NamedLocation, "Named Location"},
	{Headers::Topography, "Topography"}, {Headers::DifficultyChallenge, "Difficulty Challenge"}, {Headers::Altitude, "Altitude"}, {Headers::Temperature, "Temperature"},

	{Headers::Breakthrough1, "Breakthrough 1"}, {Headers::Breakthrough2, "Breakthrough 2"}, {Headers::Breakthrough3, "Breakthrough 3"}, {Headers::Breakthrough4, "Breakthrough 4"},
	{Headers::Breakthrough5, "Breakthrough 5"}, {Headers::Breakthrough6, "Breakthrough 6"}, {Headers::Breakthrough7, "Breakthrough 7"}, {Headers::Breakthrough8, "Breakthrough 8"},
	{Headers::Breakthrough9, "Breakthrough 9"}, {Headers::Breakthrough10, "Breakthrough 10"}, {Headers::Breakthrough11, "Breakthrough 11"}, {Headers::Breakthrough12, "Breakthrough 12"},
	{Headers::Breakthrough13, "Breakthrough 13"}
	};

	static const std::vector<Header::Headers> minRequiredHeaders{
		Headers::LatitudeDeg, Headers::LatitudeNS, Headers::LongitudeDeg, Headers::LongitudeEW,
		Headers::Metals, Headers::RareMetals, Headers::Concrete, Headers::Water,
		Headers::DustDevils, Headers::DustStorms, Headers::Meteors, Headers::ColdWaves,

		Headers::Breakthrough1, Headers::Breakthrough2, Headers::Breakthrough3, Headers::Breakthrough4, Headers::Breakthrough5,
		Headers::Breakthrough6, Headers::Breakthrough7, Headers::Breakthrough8, Headers::Breakthrough9, Headers::Breakthrough10,
		Headers::Breakthrough11, Headers::Breakthrough12, Headers::Breakthrough13
	};
}