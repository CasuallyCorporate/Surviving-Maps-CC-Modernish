#pragma once

#include <optional>

#include "Site/Headers.hpp"

namespace RequestData {
	enum SortingCategory {
		Location_Coord,
		Disasters,
		Resources
	};

	enum Comparitor {
		LessEqual,
		Equal,
		MoreEqual
	};

	static std::optional<Comparitor> getComparitor(std::string string) {
		if (string == "Less/Equal:") {
			return std::optional<Comparitor>{Comparitor::LessEqual};
		}
		if (string == "Equal:") {
			return std::optional<Comparitor>{Comparitor::Equal};
		}
		if (string == "More/Equal:") {
			return std::optional<Comparitor>{Comparitor::MoreEqual};
		}
		return {};
	}

	struct SortingRequest {
		std::optional<Header::Headers> SortItem;
		std::optional<SortingCategory> SortCategory;
		bool isLowest;
	};

	struct ValueRequest {
		int value;
		Comparitor comparitor;
	};

	struct PageSimple {
		std::optional<ValueRequest> Disasters;
		std::optional<ValueRequest> Resources;
	};

	struct ComplexResources {
		std::optional<ValueRequest> Concrete;
		std::optional<ValueRequest> Metal;
		std::optional<ValueRequest> RareMetal;
		std::optional<ValueRequest> Water;
	};

	struct ComplexDisasters {
		std::optional<ValueRequest> ColdWaves;
		std::optional<ValueRequest> DustDevils;
		std::optional<ValueRequest> DustStorms;
		std::optional<ValueRequest> Meteors;
	};

	struct PageComplex {
		std::optional<ComplexDisasters> Disasters;
		std::optional<ComplexResources> Resources;
		std::optional<ValueRequest> MapChallenge;
		std::optional<std::vector<std::string>> MapNames;
		std::optional<std::vector<std::string>> MapNamedAreas;
		std::optional<std::vector<std::string>> MapTopographies;
	};
}