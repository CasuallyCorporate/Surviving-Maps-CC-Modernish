#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iterator>
#include <syncstream>

#include "IErrors.hpp"
#include "Breakthroughs.hpp"
#include "Site/Site.hpp"

#include "VariantData/DataFlatStore.hpp"

class BreakthoughThreadStatus: IErrors {
private:
	std::mutex _workerMtx;
	std::mutex _statusMtx;
	bool _amRunning = false;
	std::mutex _errorMtx;

public:
	BreakthoughThreadStatus() { setStatusRunning(true); };
	void setStatusRunning(bool status) {
		std::lock_guard workerLock(_workerMtx);
		_amRunning = status;
	}
	bool isRunning() {
		std::lock_guard statusLock(_statusMtx);
		std::lock_guard workerLock(_workerMtx);
		return _amRunning;
	}

	void setError(Errors::ErrorMessage error) {
		std::lock_guard errorLock(_errorMtx);
		setErrorMessage(error);
	}

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override {
		return getErrorMessage();
	}
	bool hasErroredPublic() override {
		return hasErrored();
	}
};

class BreakthroughSearch {
private:
	std::vector<Breakthroughs::breakthrough_Enum> _searchBreakthroughs;
	std::vector< std::shared_ptr<BreakthroughSearch> > _searchSearches;
	std::mutex _searchMtx;
	std::mutex _insertMtx;

	std::vector<uint16_t> _siteIDs;
	std::mutex _siteMtx;

	bool searchEnums(Breakthroughs::breakthrough_Enum btrEnum, uint16_t* index) {
		if (_searchBreakthroughs.size() < 1)
		{
			*index = 0;
			return false;
		}

		uint16_t low = 0;
		uint16_t mid = 0;
		uint16_t high = _searchBreakthroughs.size() - 1;

		while (low <= high)
		{
			mid = low + (high - low) / 2;

			if (_searchBreakthroughs.at(mid) == btrEnum)
			{
				*index = mid;
				return true;
			}

			if (_searchBreakthroughs.at(mid) < btrEnum)
			{
				low = mid + 1;
			}
			else {
				if (mid == 0) {
					break;
				}
				high = mid - 1;
			}
		}

		*index = mid;
		return false;
	}

	bool getBreakthroughIndex(Breakthroughs::breakthrough_Enum btrEnum, uint16_t* index) {
		uint16_t tmpIndex;
		//binary search
		if (searchEnums(btrEnum, &tmpIndex))
		{
			*index = tmpIndex;
			return true;
		}

		return false;
	}

	bool addBreakthrough(Breakthroughs::breakthrough_Enum btrEnum) {
		uint16_t tmpIndex;
		//binary search
		if (!searchEnums(btrEnum, &tmpIndex))
		{
			if (_searchBreakthroughs.size() < 1)
			{
				_searchBreakthroughs.insert(_searchBreakthroughs.begin(), btrEnum);
				_searchSearches.insert(_searchSearches.begin(), std::make_shared<BreakthroughSearch>());
			}
			else {
				if (_searchBreakthroughs.at(tmpIndex) > btrEnum)
				{
					_searchBreakthroughs.insert(_searchBreakthroughs.begin() + tmpIndex, btrEnum);
					_searchSearches.insert(_searchSearches.begin() + tmpIndex, std::make_shared<BreakthroughSearch>());
				}
				else
				{
					_searchBreakthroughs.insert(_searchBreakthroughs.begin() + tmpIndex + 1, btrEnum);
					_searchSearches.insert(_searchSearches.begin() + tmpIndex + 1, std::make_shared<BreakthroughSearch>());
				}
			}
			return true;
		}
		else {
			return true;
		}

		return false;
	}

	bool addSite(uint16_t siteID) {
		{ // site mutex scope
			std::lock_guard lock(_siteMtx);

			int indexInsert = 0;
			for (int i = _siteIDs.size() - 1; i >= 0; i--)
			{
				if (_siteIDs.at(i) < siteID)
				{
					_siteIDs.insert(_siteIDs.begin() + i, siteID);
					indexInsert = i;
					return true;
				}
			}
			_siteIDs.insert(_siteIDs.begin(), siteID);
			indexInsert = 0;

			size_t siteIndexSize = _siteIDs.size() - 1;
			// check insert locations local 
			if (_siteIDs.size() > 1)
			{
				if (siteIndexSize - indexInsert > 0) {
					// there is an index above this that can be checked against
					if (_siteIDs.at(indexInsert + 1) <= siteID)
					{
						return false;
					}
				}
				return true;
			}
			else {
				return true;
			}
		}

		return false;
	}
public:
	bool addSiteReturnBreakthroughSearch(uint16_t siteID, Breakthroughs::breakthrough_Enum btrEnum, BreakthroughSearch** btrSearch) {
		BreakthroughSearch* tmpSearch;
		{ // search mutex scope
			std::lock_guard lock(_searchMtx);

			// Add breakthrough
			if (!addBreakthrough(btrEnum)) {
				return false;
			}
			// get/set breakthroughSearch
			uint16_t btrIndex;
			if (!getBreakthroughIndex(btrEnum, &btrIndex)) {
				return false;
			}

			try
			{
				tmpSearch = _searchSearches.at(btrIndex).get();
				*btrSearch = tmpSearch;
				return true;
			}
			catch (const std::exception&)
			{
				return false;
			}
		}

		// add the site to the next
		if (tmpSearch->addSite(siteID)) {
			return true;
		}

		return false;
	}

	/*
	std::vector<uint16_t>* getSitesPtr() {
		return &_siteIDs;
	}
	*/
};

class threadWorkContainer
{
public:
	bool isWorkable = false;

	std::vector< std::vector<int> >* btrInputPatterns;
	BreakthroughSearch* rootSearchmap;

	uint16_t siteID;
	DataFlatStore* flatStore;
	IErrors* errorObject;

	BreakthoughThreadStatus status = BreakthoughThreadStatus();
};

class WorkServer {
private:
	int _indexStart = 0;
	int _indexEnd = 0;
	int _indexat = 0;
	std::mutex _mtxGetNext;

	DataFlatStore* _flatStore;
	std::vector< std::vector<int> >* _btrPatterns;
	BreakthroughSearch* _rootSearchmap;

public:

	WorkServer(DataFlatStore* flatStore, std::vector< std::vector<int> >* btrInputPatterns, BreakthroughSearch* rootSearchmap) {
		_flatStore = flatStore;
		_btrPatterns = btrInputPatterns;
		_rootSearchmap = rootSearchmap;
	}

	void setIndexes(int first, int last) {
		_indexStart = first;
		_indexEnd = last;
	}

	std::shared_ptr<threadWorkContainer> getNewWorkItem() {
		std::lock_guard lock(_mtxGetNext);

		std::shared_ptr <threadWorkContainer> tmpContainer = std::make_shared<threadWorkContainer>();

		// Check if we are at or beyond the final index
		if (_indexat >= _indexEnd)
		{
			tmpContainer->isWorkable = false;
			return tmpContainer;
		}

		tmpContainer->isWorkable = true;
		tmpContainer->btrInputPatterns = _btrPatterns;
		tmpContainer->rootSearchmap = _rootSearchmap;
		tmpContainer->siteID = _indexat;
		tmpContainer->flatStore = _flatStore;

		_indexat++;
		return tmpContainer;
	}
};


/* DataBreakthough
* 
*  This class is responsible for holding the search structure for breakthroughs
*/
class DataBreakthrough: IErrors {
private:

	std::shared_ptr<BreakthroughSearch> _searchMapBreakthroughs;
	int _breakthroughArrayLength = -1;
	std::vector< std::vector<int> > _indexSelectionPatterns;

	std::mutex _mtxErrors;

	bool generatePatternIntoVector(const int parentSize, const int selections, std::vector< std::vector<int> >* indexPatterns) {
		if (selections < 1) { setErrorMessage( {"selections size incorrect. Less than 1", Errors::ToFrom::None, Errors::ToFrom::None} ); return false; }
		if (parentSize < 1) { setErrorMessage( { "parentSize incorrect. Less than 1", Errors::ToFrom::None, Errors::ToFrom::None } ); return false; }
		if (selections > parentSize) { setErrorMessage( { "selections is above parentSize", Errors::ToFrom::None, Errors::ToFrom::None } ); return false; }
		std::vector<std::pair<size_t, size_t>> ItemIndex_PrevIndex{}; // index, prevIndex
		std::vector<int> tempResult{};
		// Initial placement
		for (size_t i = 0; i < selections; i++)
		{
			ItemIndex_PrevIndex.emplace_back(std::pair<size_t, size_t>(i, i));
			tempResult.emplace_back(i);
		}

		// Start moving things around for the combinations
		size_t parentIndexSize = parentSize - 1;
		size_t indexSize = ItemIndex_PrevIndex.size() - 1;
		size_t endIndex = parentIndexSize - indexSize;
		// Item-Index to move, end-Item-Index
		std::pair<size_t, size_t> MovingItem_EndIndex = std::pair<size_t, size_t>(0, endIndex);

		// emplace starting point
		for (size_t i = 0; i < ItemIndex_PrevIndex.size(); i++)
		{
			tempResult.at(i) = ItemIndex_PrevIndex.at(i).first;
		}
		indexPatterns->emplace_back(tempResult);

		// while the first itemIndex has not yet reached it's end state
		while (ItemIndex_PrevIndex.at(0).first < endIndex) {
			// Check if the next item over is next to this MovingItem. If so swap to the next one instead
			if (MovingItem_EndIndex.first < indexSize)
			{ // Is item the end item? No
				if (ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first != MovingItem_EndIndex.second)
				{ // if we're at our target index, we need to reset any in-between items and skip moving forward
					if (ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first + 1 == ItemIndex_PrevIndex.at(MovingItem_EndIndex.first + 1).first)
					{
						MovingItem_EndIndex.first++;
						MovingItem_EndIndex.second++;
						continue; // Start the 'while' again
					}
				}
			}

			if (ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first != MovingItem_EndIndex.second) {
				ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first++;
				for (size_t i = 0; i < ItemIndex_PrevIndex.size(); i++)
				{
					tempResult.at(i) = ItemIndex_PrevIndex.at(i).first;
				}
				indexPatterns->emplace_back(tempResult);
				continue;
			}
			else { // We have reached the EndIndex for this movingItem
				if (MovingItem_EndIndex.first > 0) { // If we are not the first item
					// Reset + 2?
					if (ItemIndex_PrevIndex.at(MovingItem_EndIndex.first - 1).first + 2 <= MovingItem_EndIndex.second)
					{ // Move it to prev-Item +2 (if possible)
						ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first = ItemIndex_PrevIndex.at(MovingItem_EndIndex.first - 1).first + 2;
						// If we have items above. We need to reset them to +n of the this item. Now we are in the correct place
						for (size_t k = 1; k + MovingItem_EndIndex.first <= indexSize; k++)
						{
							ItemIndex_PrevIndex.at(MovingItem_EndIndex.first + k).first = ItemIndex_PrevIndex.at(MovingItem_EndIndex.first).first + k;
						}
					}
				}
				else { // We are the first item, and we have reached the EndIndex
					// We can exit
					break;
				}
			}

			// Decrement
			MovingItem_EndIndex.first--;
			MovingItem_EndIndex.second--;
		}

		return true;
	}

public:
	void threadedAddSearchResults(WorkServer* workServer) {

		// Get new workOrder
		std::shared_ptr<threadWorkContainer> workOrder = workServer->getNewWorkItem();

		// Get the site
		Site* workingSite;
		while (workOrder->isWorkable) {
			if (!workOrder->flatStore->getSite(workOrder->siteID, &workingSite))
			{
				workOrder->status.setError({ "Could not retieve site from flatStore", Errors::ToFrom::Read, Errors::ToFrom::None });
				workOrder->status.setStatusRunning(false);
				return;
			}

			workOrder->status.setStatusRunning(true);
			std::osyncstream(std::cout) << std::this_thread::get_id() << " Started work for site:" << workOrder->siteID << std::endl;
			// sort from smallest to largest enum for the site's breakthroughs
			std::vector<Breakthroughs::breakthrough_Enum> items = workingSite->getAllBreakthroughs();
			size_t btrArrayLength = items.size();
			std::sort(items.begin(), items.end(), std::less<Breakthroughs::breakthrough_Enum>());
			// check for duplicates in the array
			if (btrArrayLength < 2)
			{
				workOrder->status.setError({"Breakthroughs list too small", Errors::ToFrom::Read, Errors::ToFrom::None});
				workOrder->status.setStatusRunning(false);
				return;
			}
			for (size_t i = 1; i < items.size(); i++) {
				if (items.at(i) == items.at(i - 1))
				{
					workOrder->status.setError({ "Breakthroughs list contains duplicates", Errors::ToFrom::Read, Errors::ToFrom::None });
					workOrder->status.setStatusRunning(false);
					return;
				}
			}
			if (btrArrayLength != items.size())
			{
				workOrder->status.setError({ "Breakthroughs list size is not the same size as the original", Errors::ToFrom::Read, Errors::ToFrom::None });
				workOrder->status.setStatusRunning(false);
				return;
			}

			// check if we have the full selection patterns, if not generate them
			if (workOrder->btrInputPatterns->size() == 0)
			{
				workOrder->status.setError({ "Breakthrough patterns do not exist", Errors::ToFrom::Read, Errors::ToFrom::None });
				workOrder->status.setStatusRunning(false);
				return;
			}

			// insert all possible breakthrough selections into the breakthrough search
			size_t selectionsSize = workOrder->btrInputPatterns->size();
			BreakthroughSearch* searchMap = nullptr;
			BreakthroughSearch* tmpMap = nullptr;

			for (size_t i = 0; i < selectionsSize; i++)
			{ // for each pattern, insert the pattern into the map
				// Reset the searchmap to the root
				searchMap = workOrder->rootSearchmap;

				// For reach selectionIndex, insert it and get the next index (or nullptr)
				for (int selectionIndex : workOrder->btrInputPatterns->at(i))
				{
					// Extract the breakthough from the array
					Breakthroughs::breakthrough_Enum tmpEnum = items.at(selectionIndex);

					if (!searchMap->addSiteReturnBreakthroughSearch(workOrder->siteID, tmpEnum, &tmpMap))
					{
						workOrder->status.setError({ "BreakthroughSearch returned false" , Errors::ToFrom::Create, Errors::ToFrom::None });
						workOrder->status.setStatusRunning(false);
						return;
					}
					searchMap = tmpMap;
				}
			}

			std::osyncstream(std::cout) << std::this_thread::get_id() << " Values for site:" << workOrder->siteID << " input into DataBreakthrough, on thread:" << std::endl;

			// get the next work order
			workOrder = workServer->getNewWorkItem();
		}
	}

	DataBreakthrough() {
		_searchMapBreakthroughs = std::make_shared<BreakthroughSearch>();
	}

	bool generateSearchTree(DataFlatStore* flatStore) {

		// pre-generate search patterns
		if (flatStore->getSites() < 1)
		{
			std::lock_guard lock(_mtxErrors);
			setErrorMessage( {"Flatstore sites < 1", Errors::ToFrom::Read, Errors::ToFrom::None} );
			return false;
		}
		Site* tempSite = nullptr;
		if (!flatStore->getSite(0, &tempSite)) {
			std::lock_guard lock(_mtxErrors);
			setErrorMessage( {"Could not get site from flatstore", Errors::ToFrom::Read, Errors::ToFrom::None} );
			return false;
		}
		_breakthroughArrayLength = tempSite->getAllBreakthroughs().size();
		// generate searchPatterns
		for (size_t btrLen = 1; btrLen <= _breakthroughArrayLength; btrLen++)
		{
			if (!generatePatternIntoVector(_breakthroughArrayLength, btrLen, &_indexSelectionPatterns)) {
				std::lock_guard lock(_mtxErrors);
				setErrorMessage( {"Could not generate Breakthrough patterns", Errors::ToFrom::Read, Errors::ToFrom::None} );
				return false;
			}
		}

		// Set WorkServer
		WorkServer workServer = WorkServer(flatStore, &_indexSelectionPatterns, _searchMapBreakthroughs.get());
		workServer.setIndexes(0, 12);

		const unsigned int procCount = std::thread::hardware_concurrency();
		std::vector<std::thread> threadPool;
		threadPool.reserve(procCount);

		size_t threadsEngaged = 0;

		std::osyncstream(std::cout) << "DataBreakthrough: starting threads" << std::endl;

		// start off all threads
		while (threadsEngaged < procCount)
		{
			// add a thread
			// if not all threads have been added to the threadPool
			if (threadPool.size() < procCount)
			{ // Add a new threadpool
				threadPool.emplace_back( std::thread(
					&DataBreakthrough::threadedAddSearchResults,
					this,
					&workServer
				));

				threadsEngaged++;
				continue;
			}
		}

		// rejoin all threads
		std::osyncstream(std::cout) << "DataBreakthrough: rejoining threads" << std::endl;
		while (threadPool.size() > 0)
		{
			threadPool.back().join();
			threadPool.pop_back();
		}
		std::osyncstream(std::cout) << "DataBreakthrough: threads rejoined" << std::endl;

		std::lock_guard lock(_mtxErrors);
		if (hasErrored())
		{
			return false;
		}

		return true;
	}

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override {
		return getErrorMessage();
	}
	bool hasErroredPublic() override {
		return hasErrored();
	}
};