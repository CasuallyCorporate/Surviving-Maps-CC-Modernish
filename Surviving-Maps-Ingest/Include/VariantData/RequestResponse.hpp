#pragma once

namespace RequestResponse {
	static const std::string ErrorJsonParseResponse = "{ \"Response\":\"Error: JSON parse\"}";

	static std::string ErrorProcessing = "{ \"Response\":\"Error: Request processing encountered an error\"}";
	static std::string ErrorNoResults = "{ \"Response\":\"Error: No results for selected query\"}";
	static std::string ErrorReqDataInvalid = "{ \"Response\":\"Error: Request contains invalid data\"}";
}