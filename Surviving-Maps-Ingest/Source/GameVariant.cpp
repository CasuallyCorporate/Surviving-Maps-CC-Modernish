#include "GameVariant.hpp"

//Public

GameVariant::GameVariant(std::string variant, std::string csvURL) {
	_variant = std::filesystem::path(variant);
	_csvURL = std::filesystem::path(csvURL);
}

bool GameVariant::ingestCSV() {
	if (std::filesystem::exists(_csvURL))
	{
		//Setup datas
		_FlatData = std::make_unique<DataFlatStore>();
		_CoordinateData = std::make_unique<DataCoordinate>();
		_BreakthroughData = std::make_unique<DataBreakthrough>();

		CSVIngest csvIngestor = CSVIngest(_csvURL.generic_string());
		if (!csvIngestor.populateFlatData(_FlatData.get()))
		{
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

bool GameVariant::processDatas() {
	// Check the FlatData has been populated with at least some data
	if (_FlatData.get()->getMaxSites() < 1)
	{
		setErrorMessage({ "FlatDat does not have any entries", Errors::ToFrom::Create, Errors::ToFrom::None });
		return false;
	}

	// Destroy any existing folder structure for this GameVariant
	std::filesystem::path cwd = std::filesystem::current_path().generic_string();
	if (std::filesystem::exists(cwd / _variant)) {
		if (std::filesystem::remove_all(cwd / _variant) < 1) {
			setErrorMessage({ _variant.generic_string() + " exists, but was not removed", Errors::ToFrom::Delete, Errors::ToFrom::None});
			return false;
		}
	}
	// Create this GameVariants Root folder
	std::filesystem::path variantRoot = cwd / _variant;
	if (!std::filesystem::create_directory(variantRoot)) {
		setErrorMessage({ "Directory " + _variant.generic_string() + " was not re-created", Errors::ToFrom::Create, Errors::ToFrom::None });
		return false;
	}

	// Process the FlatData into a datafile


	// Process FlatDat into breakthrough data
	if (!_BreakthroughData.get()->generateSearchTree(_FlatData.get()))
	{
		setErrorMessage(_BreakthroughData.get()->getErrorPublic());
	}

	// process FlatDat into coordinate data

	return false;
}

// IErrored publics
Errors::ErrorMessage GameVariant::getErrorPublic() {
	return getErrorMessage();
}
bool GameVariant::hasErroredPublic() {
	return hasErrored();
}