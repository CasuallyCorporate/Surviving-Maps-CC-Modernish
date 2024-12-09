#include "CSVIngest.hpp"

// private
void CSVIngest::csvLineToVector(std::string* str, std::vector<std::string>* vect) {
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
bool CSVIngest::stringToInt(std::string* string, int* num) {
	try
	{
		*num = std::atoi(string->c_str());
		return true;
	}
	catch (const std::exception&)
	{
		setErrorMessage( {"StringToInt was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
		return false;
	}
}
bool CSVIngest::stringToChar(std::string* string, char* singleChar) {
	try
	{
		*singleChar = string->at(0);
		return true;
	}
	catch (const std::exception&)
	{
		setErrorMessage( {"StringToChar was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
		return false;
	}
}

// public
CSVIngest::CSVIngest(std::string csvURL) {
	_csvURL = csvURL;
}

bool CSVIngest::populateFlatData(DataFlatStore* datFlat) {
	std::ifstream csvFile(_csvURL);
	std::string line;
	// First line is csv header
	try
	{
		std::getline(csvFile, line);
	}
	catch (const std::exception&)
	{
		setErrorMessage( {"Could not read csvURL", Errors::ToFrom::Read, Errors::ToFrom::None} );
		return false;
	}

	std::vector<std::string> lineItems;
	csvLineToVector(&line, &lineItems);

	// Parse header to column map
	std::map<Sites::Header, int> csvHeaderColumn;
	int itemIndex = 0;
	for (std::string column: lineItems) {
		Sites::Header hdr = Sites::Header::None;

		if (auto search = Sites::stringToHeader.find(column); search != Sites::stringToHeader.end()) {
			csvHeaderColumn[search->second] = itemIndex;
		}
		itemIndex++;
	}

	// TODO: Check header column map for the required headers
	std::vector<Sites::Header> requiredHeaders = Sites::minRequiredHeaders;
	std::map<Sites::Header, int>::iterator mapIt;
	for (mapIt = csvHeaderColumn.begin(); mapIt != csvHeaderColumn.end(); mapIt++) {
		std::vector<Sites::Header>::iterator reqIt = std::find(requiredHeaders.begin(), requiredHeaders.end(), mapIt->first);

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
		setErrorMessage( {"Minimum headers do not exist", Errors::ToFrom::Read, Errors::ToFrom::None} );
		return false;
	}

	// Site thingy
	Site tmpSite;

	// Deal with the rest of the csv, into the tempSite
	int recNum = 1;
	int tmpNum;
	char tmpChar;
	int sitenum = -1;
	while (std::getline(csvFile, line)) {
		csvLineToVector(&line, &lineItems);
		tmpSite.transferContentsFrom(Site());

		for (mapIt = csvHeaderColumn.begin(); mapIt != csvHeaderColumn.end(); mapIt++)
		{
			switch (mapIt->first)
			{
#pragma region LatitudeDeg
			case Sites::Header::LatitudeDeg:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setLatitudeValue(tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( {"LatitudeDeg column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
					return false;
				}
				break;
#pragma endregion
#pragma region LatitudeNS
			case Sites::Header::LatitudeNS:
				// get char from std::string
				if (lineItems.at(mapIt->second).size() == 1)
				{
					if (stringToChar(&lineItems.at(mapIt->second), &tmpChar)) {
						tmpSite.setLatitudeNS(tmpChar);
						break;
					}
				}
				setErrorMessage( { "LatitudeNS column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None } );
				return false;
				break;
#pragma endregion
#pragma region LongitudeDeg
			case Sites::Header::LongitudeDeg:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setLongitudeValue(tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( { "LongitudeDeg column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None } );
					return false;
				}
				break;
#pragma endregion
#pragma region LongitudeEW
			case Sites::Header::LongitudeEW:
				// get char from std::string
				if (lineItems.at(mapIt->second).size() == 1)
				{
					if (stringToChar(&lineItems.at(mapIt->second), &tmpChar)) {
						tmpSite.setLongitudeEW(tmpChar);
						break;
					}
				}
				setErrorMessage( { "LongitudeEW column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None } );
				return false;
				break;
#pragma endregion
#pragma region Resources_MetalsRareMetalsConcreteWater
			case Sites::Header::Metals:
			case Sites::Header::RareMetals:
			case Sites::Header::Concrete:
			case Sites::Header::Water:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setResourceValue(mapIt->first, tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( { "Resource column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None } );
					return false;
				}
				break;
#pragma endregion
#pragma region Disasters_DustDevilsDustStormsMeteorsColdWaves
			case Sites::Header::DustDevils:
			case Sites::Header::DustStorms:
			case Sites::Header::Meteors:
			case Sites::Header::ColdWaves:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setDisasterValue(mapIt->first, tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( {"Disaster column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
					return false;
				}
				break;
#pragma endregion
#pragma region MapName
			case Sites::Header::MapName:
				tmpSite.setMapName(lineItems.at(mapIt->second));
				break;
				//,	Topography,
#pragma endregion
#pragma region NamedLocation
			case Sites::Header::NamedLocation:
				tmpSite.setMapLocation(lineItems.at(mapIt->second));
				break;
#pragma endregion
#pragma region Topography
			case Sites::Header::Topography:
				tmpSite.setMapTopography(lineItems.at(mapIt->second));
				break;
#pragma endregion
#pragma region DifficultyChallenge
			case Sites::Header::DifficultyChallenge:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setMapDifficulty(tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( { "DifficultyChallenge column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None } );
					return false;
				}
				break;
#pragma endregion
#pragma region Altitude
			case Sites::Header::Altitude:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setMapAltitude(tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( {"Altitude column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
					return false;
				}
				break;
#pragma endregion
#pragma region Temperature
			case Sites::Header::Temperature:
				// get int from std::string
				try
				{
					if (stringToInt(&lineItems.at(mapIt->second), &tmpNum)) {
						tmpSite.setMapTemperature(tmpNum);
					}
					else {
						return false;
					}
				}
				catch (const std::exception&)
				{
					setErrorMessage( {"Altitude column was not correctly parsed", Errors::ToFrom::Update, Errors::ToFrom::None} );
					return false;
				}
				break;
#pragma endregion
#pragma region Breakthroughs
			case Sites::Header::Breakthrough1:
			case Sites::Header::Breakthrough2:
			case Sites::Header::Breakthrough3:
			case Sites::Header::Breakthrough4:
			case Sites::Header::Breakthrough5:
			case Sites::Header::Breakthrough6:
			case Sites::Header::Breakthrough7:
			case Sites::Header::Breakthrough8:
			case Sites::Header::Breakthrough9:
			case Sites::Header::Breakthrough10:
			case Sites::Header::Breakthrough11:
			case Sites::Header::Breakthrough12:
			case Sites::Header::Breakthrough13:
			{
				// Translate text to breakthrough
				auto btrIt = Breakthroughs::stringToEnum.find(lineItems.at(mapIt->second));

				if (btrIt == Breakthroughs::stringToEnum.end())
				{
					setErrorMessage( {"Breakthrough column text is not a valid breakthrough", Errors::ToFrom::Read, Errors::ToFrom::None} );
					return false;
				}
				else {
					// Add breakthrough
					if (!tmpSite.setBreakthrough(mapIt->first, btrIt->second)) {
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

		// Succesfully parsed. Add to Stores
		if (!datFlat->addSite(&tmpSite, &sitenum)) {
			setErrorMessage( datFlat->getErrorPublic() );
			return false;
		}
	}

	if (sitenum != datFlat->getMaxSites()) {
		// expecting 50900 sites
		return false;
	}

	return true;
}

// IErrored publics
Errors::ErrorMessage CSVIngest::getErrorPublic() {
	return getErrorMessage();
}
bool CSVIngest::hasErroredPublic() {
	return hasErrored();
}