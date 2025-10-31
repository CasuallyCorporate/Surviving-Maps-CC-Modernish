#include <iostream>
#include <syncstream>
#include <memory>
#include <vector>
#include <thread>
#include <csignal>

#include <httplib.h>

#include "IErrors.hpp"
#include "GameVariant.hpp"

/* Main
*  - Holds the variants themselves in memory
*  - Starts CSV ingests
*  - provides boost::beast search capability
*/

std::map<std::string, std::unique_ptr<GameVariant>> _variantMap;
IErrors _error_obj;

std::vector<std::thread> _ingestThreads;

httplib::Server* _SrvPointer = nullptr;

void stopSrvSignalHandler(int sig) {
	if (_SrvPointer) {
		_SrvPointer->stop();
	}
}

int main(/*int args, char* argv[]*/) {
	std::cout << "<!! Setting variants !!>\n";

	// Insert all variants to process
	_variantMap["STANDARD"] = std::make_unique<GameVariant>("STANDARD", "CSVMapData/MapData Breakthroughs.csv", &_error_obj);
	_variantMap["GREEN_PLANET"] = std::make_unique<GameVariant>("GREEN_PLANET", "CSVMapData/MapData Breakthroughs GP.csv", &_error_obj);
	_variantMap["BELOW_BEYOND"] = std::make_unique<GameVariant>("BELOW_BEYOND", "CSVMapData/MapData Breakthroughs BB.csv", &_error_obj);
	_variantMap["BEYOND_GREEN"] = std::make_unique<GameVariant>("BEYOND_GREEN", "CSVMapData/MapData Breakthroughs GP BB.csv", &_error_obj);
	_variantMap["TITO_GREEN_PLANET"] = std::make_unique<GameVariant>("TITO_GREEN_PLANET", "CSVMapData/MapData Tito GP.csv", &_error_obj);
	_variantMap["EVANS_GREEN_PLANET"] = std::make_unique<GameVariant>("EVANS_GREEN_PLANET", "CSVMapData/MapData Evans GP.csv", &_error_obj);

	std::cout << "<!! Starting variants' ingest !!>\n";
	// process all map data into their variants
	std::map<std::string, std::unique_ptr<GameVariant>>::iterator it;
	for (it = _variantMap.begin(); it != _variantMap.end(); it++) {
		// Ingest CSV
		std::cout << "<--- Ingesting " << it->first << " -->" << std::endl;
		_ingestThreads.emplace_back(
			std::thread(
				&GameVariant::ingest,
				it->second.get()
			)
		);
	}
	while (!_ingestThreads.empty()) {
		_ingestThreads.back().join();
		_ingestThreads.pop_back();
	}
	if (_error_obj.hasErrored()) {
		return -1;
	}

	std::cout << "<!! Starting server !!>\n";
	// setup and start httplib server

	httplib::Server srv;
	size_t hw_conc = std::thread::hardware_concurrency();
	if (hw_conc > 1) {
		hw_conc--;
		srv.new_task_queue = [&hw_conc] { return new httplib::ThreadPool(hw_conc); };
	} else {
		srv.new_task_queue = [] { return new httplib::ThreadPool(1); };
	}

	srv.Post("/motd", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{ \"Response\":\"Not implemented\"}", "application/json");
	});

	srv.Post("/site", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{ \"Response\":\"Not implemented\"}", "application/json");
	});

	srv.Post("/page", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{ \"Response\":\"Not implemented\"}", "application/json");
	});

	srv.Post("/coord", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("{ \"Response\":\"Not implemented\"}", "application/json");
	});

	// setup stop signals
	_SrvPointer = &srv;
	signal(SIGABRT, stopSrvSignalHandler);
	signal(SIGTERM, stopSrvSignalHandler);
	signal(SIGINT, stopSrvSignalHandler);

	std::cout << "<!! Server Listening !!>\n";
	srv.listen("0.0.0.0", 8000);

	return 0;
}