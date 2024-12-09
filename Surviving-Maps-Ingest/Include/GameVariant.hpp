#pragma once

#include <memory>
#include <string>

#include <filesystem>

#include "IErrors.hpp"
#include "Breakthroughs.hpp"
#include "CSVIngest.hpp"

#include "VariantData/DataFlatStore.hpp"
#include "VariantData/DataCoordinate.hpp"
#include "VariantData/DataBreakthrough.hpp"

class GameVariant: IErrors {
private:
	std::filesystem::path _variant, _csvURL;

	//Datas
	std::unique_ptr<DataFlatStore> _FlatData;
	std::unique_ptr<DataCoordinate> _CoordinateData;
	std::unique_ptr<DataBreakthrough> _BreakthroughData;

public:
	GameVariant(std::string variant, std::string csvURL);

	bool ingestCSV();

	bool processDatas();

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override;
	bool hasErroredPublic() override;
};