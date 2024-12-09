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

#include "VariantData/DataBreakthrough.hpp"
#include "VariantData/DataCoordinate.hpp"
#include "VariantData/DataFlatStore.hpp"

class CSVIngest: IErrors {
private:

	std::string _csvURL;
	void csvLineToVector(std::string* str, std::vector<std::string>* vect);
	bool stringToInt(std::string* str, int* num);
	bool stringToChar(std::string* str, char* singleChar);
public:
	CSVIngest(std::string csvURL);

	bool populateFlatData(DataFlatStore* datFlat);

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override;
	bool hasErroredPublic() override;
};