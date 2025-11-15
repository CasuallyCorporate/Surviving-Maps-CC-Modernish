#include <optional>

#include "Site/Headers.hpp"

namespace RequestData {
	enum Comparitor {
		LessEqual,
		Equal,
		MoreEqual
	};

	static std::optional<Comparitor> getComparitor(std::string string) {
		if (string == "Less/Equal:") {
			return LessEqual;
		}
		if (string == "Equal:") {
			return Equal;
		}
		if (string == "More/Equal:") {
			return MoreEqual;
		}
		return {};
	}

	struct Sorting {
		Header::Headers SortItem;
		bool isLowest;
	};

	struct ValueComparitor {
		int value;
		Comparitor comparitor;
	};

	struct PageSimple {
		std::optional<ValueComparitor> Disasters;
		std::optional<ValueComparitor> Resources;
	};

	struct ComplexResources {
		std::optional<ValueComparitor> Concrete;
		std::optional<ValueComparitor> Metal;
		std::optional<ValueComparitor> RareMetal;
		std::optional<ValueComparitor> Water;
	};

	struct ComplexDisasters {
		std::optional<ValueComparitor> ColdWaves;
		std::optional<ValueComparitor> DustDevils;
		std::optional<ValueComparitor> DustStorms;
		std::optional<ValueComparitor> Meteors;
	};

	struct PageComplex {
		std::optional<ComplexDisasters> Disasters;
		std::optional<ComplexResources> Resources;
	};
}