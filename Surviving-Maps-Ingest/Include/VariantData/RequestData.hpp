#pragma once

#include <optional>

#include "Site/Headers.hpp"

namespace RequestData {
	enum SortingCategory {
		Location_Coord,
		DisastersTotal,
		ResourcesTotal
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

	class IndexCollation {
	private:
		std::vector<uint16_t>* _vecTot;
		std::vector<uint16_t> _inner;
		uint16_t _maxIndex;
		std::vector<uint16_t>::iterator _it;
		uint16_t _nonIt = 0;
		bool _hasMoved = false, _isIncremental = false;
	public:
		IndexCollation(std::vector<uint16_t>* vecTot, uint16_t maxIndex) {
			_vecTot = vecTot;
			_maxIndex = maxIndex;
			_it = _vecTot->begin();
			_isIncremental = (_it == _vecTot->end());
		}

		void move(uint16_t value) {
			_inner.emplace_back(value);
			_hasMoved = true;
		}

		bool nextValue(uint16_t* retValue) {
			if (_isIncremental) {
				// Go through until max
				if (_nonIt < _maxIndex) {
					*retValue = _nonIt;
					_nonIt++;
					return true;
				}
				return false;
			}
			else {
				if (_it != _vecTot->end()) {
					*retValue = *_it;
					_it++;
					return true;
				}
				return false;
			}
		}

		bool checkMovedReconcileReset() {

			*_vecTot = _inner;

			// Reset and return
			if (_hasMoved) {
				_it = _vecTot->begin();
				_isIncremental = (_it == _vecTot->end());
				_inner.clear();
				_nonIt = 0;

				return true;
			}
			return false;
		}
	};
}