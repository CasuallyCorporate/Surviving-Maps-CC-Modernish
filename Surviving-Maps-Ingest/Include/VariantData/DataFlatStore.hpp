#pragma once

#include <array>
#include <memory>
#include <mutex>

#include "Site/Site.hpp"

#include "IErrors.hpp"

/* DataFlatStore
* 
*  Stores each site that comes in to a simple vector.
*  The index of each site essentially becomes it's ID
*/

class DataFlatStore: IErrors {
private:
	std::unique_ptr<std::array<Site, 50901>> _sites;
	int _sitesNum = -1;
	std::mutex _mtxGetSite;
public:
	DataFlatStore() {
		try
		{
			_sites = std::make_unique<std::array<Site, 50901>>();
		}
		catch (const std::exception&)
		{
			setErrorMessage( {"Could not create DataFlatStore. Likely out of memory", Errors::ToFrom::Create, Errors::ToFrom::None} );
		}
	};

	/// <summary>
	/// Stores a site into the FlatStore. Returns the index into which it was stored with another pointer.
	/// </summary>
	/// <param name="site">The site which will be added to the FlatStore</param>
	/// <param name="sitenum">Returns the index into which the site has been entered to this pointer</param>
	/// <returns></returns>
	bool addSite(Site* site, int* sitenum) {
		if ((_sitesNum + 1) < _sites.get()->size())
		{
			_sites->at(_sitesNum + 1).transferContentsFrom(*site);
			_sitesNum++;
			*sitenum = _sitesNum;
			return true;
		}
		setErrorMessage( { "Could not add Site. Array full", Errors::ToFrom::Create, Errors::ToFrom::None } );
		return false;
	};
	bool getSite(int siteID, Site** site) {
		std::lock_guard lock(_mtxGetSite);
		if (siteID < _sites.get()->size() && siteID <= _sitesNum)
		{
			*site = &_sites.get()->at(siteID);
			return true;
		}
		site = nullptr;
		return false;
	};

	int getSites() {
		return _sitesNum;
	}
	int getMaxSites() {
		return _sites.get()->size() - 1;
	}

	// IErrored publics
	Errors::ErrorMessage getErrorPublic() override {
		return getErrorMessage();
	}
	bool hasErroredPublic() override {
		return hasErrored();
	}
};