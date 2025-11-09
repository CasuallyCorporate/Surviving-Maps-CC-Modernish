#include <iostream>
#include <syncstream>
#include <memory>
#include <vector>
#include <thread>
#include <csignal>
#include <shared_mutex>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "IErrors.hpp"
#include "GameVariant.hpp"
#include "ingestChangeFaq.hpp"

using json = nlohmann::json;

/* Main
*  - Holds the variants themselves in memory
*  - Starts CSV ingests
*  - provides boost::beast search capability
*/

std::map<std::string, std::unique_ptr<GameVariant>> _variantMap;
IErrors _error_obj;

std::vector<std::thread> _ingestThreads;

// Cached json change/faq
json _faq = json({});
json _changelog = json({});

httplib::Server* _SrvPointer = nullptr;

void stopSrvSignalHandler(int sig) {
	if (_SrvPointer) {
		_SrvPointer->stop();
	}
}

// Json checks
bool checkRequestMOTD(json& reqjson, json** returnJson) {
	auto page = reqjson.find("page");
	if (page != reqjson.end()) {
		if (page.value().is_string()) {
			std::string strval;
			page.value().get_to(strval);

			if (strval == "Changelog") {
				*returnJson = &_changelog;
				return true;
			}
			if (strval == "FAQ") {
				*returnJson = &_faq;
				return true;
			}
		}
	}
	return false;
}

bool checkRequestSite(json& reqjson) {
	if (auto resp = reqjson.find("SiteID"); resp != reqjson.end()) {
		if (!resp.value().is_number()) {
			return false;
		}
		/* fall-through */
	}
	else return false;

	if (auto resp = reqjson.find("VariantSystem"); resp != reqjson.end()) {
		if (!resp.value().is_string()) {
			return false;
		}
		/* fall-through */
	}else return false;

	return true;
}

int main(/*int args, char* argv[]*/) {
	std::cout << "<!! Setting up cached/static json responses !!>\n";

	// Setup cached json responses
	if (!interperetFaqJson(&_faq)) {
		std::cout << "<!!!! Failed to ingest faq json !!!!>\n";
		return -1;
	}
	if (!iterperetChangelogJson(&_changelog)) {
		std::cout << "<!!!! Failed to ingest changelog json !!!!>\n";
		return -1;
	}

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

	const std::string MimeTypeJson = "application/json";
	const std::string ErrorParseResponse = "{ \"Response\":\"Error: JSON parse\"}";

	srv.Post("/motd", [&MimeTypeJson, &ErrorParseResponse](
		const httplib::Request& req, httplib::Response& res){
		json reqJson = json({});
		try {
			reqJson = json::parse(req.body);
		}
		catch (...) {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}

		json* retJson = nullptr;
		if (checkRequestMOTD(reqJson, &retJson)) {
			if (retJson != nullptr) {
				res.status = httplib::StatusCode::OK_200;
				res.set_content(retJson->dump(), MimeTypeJson);
				return;
			}
		}
		else {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}

		res.status = httplib::StatusCode::InternalServerError_500;
		res.set_content(ErrorParseResponse, MimeTypeJson);
		return;
	});

	srv.Post("/site", [&MimeTypeJson, &ErrorParseResponse](
		const httplib::Request& req, httplib::Response& res) {
		json reqJson = json({});
		try {
			reqJson = json::parse(req.body);
		}
		catch (...) {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}

		if (checkRequestSite(reqJson)) {
			// Check variants for VariantSystem
			auto sysResp = _variantMap.find(reqJson["VariantSystem"]);
			if (sysResp != _variantMap.end()) {
				// Get site info from variant
				json siteJson;
				if (sysResp->second->getSiteAsJson(reqJson["SiteID"], &siteJson)) {
					res.status = httplib::StatusCode::OK_200;
					res.set_content(siteJson.dump(), MimeTypeJson);
					return;
				}
			}

			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}
		else {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}

		res.status = httplib::StatusCode::InternalServerError_500;
		res.set_content(ErrorParseResponse, MimeTypeJson);
		return;
	});

	srv.Post("/page", [&MimeTypeJson, &ErrorParseResponse](
		const httplib::Request& req, httplib::Response& res) {
		json reqJson = json({});
		try {
			reqJson = json::parse(req.body);
		}
		catch (...) {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}
	});

	srv.Post("/coord", [&MimeTypeJson, &ErrorParseResponse](
		const httplib::Request& req, httplib::Response& res) {
		json reqJson = json({});
		try {
			reqJson = json::parse(req.body);
		}
		catch (...) {
			res.status = httplib::StatusCode::BadRequest_400;
			res.set_content(ErrorParseResponse, MimeTypeJson);
			return;
		}
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