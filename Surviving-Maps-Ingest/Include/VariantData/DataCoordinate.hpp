#pragma once

#include <memory>
#include <vector>

#include "IErrors.hpp"
#include "Site/Site.hpp"

class DataCoordinate: IErrors {
private:
	std::map<
		std::pair<int, int>, // coNS, coEW
		std::map< std::pair<char,char>, int> // NS, EW, siteID
	> _siteIDs;
public:
	DataCoordinate() {};

	bool addCoordinate(Site* site, int id) {
		return false;
	};
	bool getID(int* id, char NS, char EW, int coNS, int coEW);

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override {
		return getErrorMessage();
	}
	bool hasErroredPublic() override {
		return hasErrored();
	}
};