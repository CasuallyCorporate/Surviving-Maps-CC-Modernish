#pragma once
#include <nlohmann/json.hpp>

using json = nlohmann::json;

static bool iterperetChangelogJson(json* outJson) {
	try
	{
		json alljson;

		json ModernishSection = {
			{"heading","Changelog"},
			{"subheading", "CasuallyCorporate/Surviving-Maps-CC-Modernish"}
		};
		std::ifstream f("ChangeFaqData/ChangeLog-Modernish.json");
		json tmpcurrentCL = json::parse(f);
		ModernishSection["contents"] = tmpcurrentCL;
		f.close();

		json CCSection = {
			{"subheading", "CasuallyCorporate/Surviving-Maps-CC"}
		};
		f.open("ChangeFaqData/ChangeLog-CC.json");
		json tmpcurrentCC = json::parse(f);
		CCSection["contents"] = tmpcurrentCC;
		f.close();

		json TrickstersSection = {
			{"subheading", "trickster-is-weak/Surviving-Maps"}
		};
		f.open("ChangeFaqData/ChangeLog-Trickster.json");
		json tmpcurrentTS = json::parse(f);
		TrickstersSection["contents"] = tmpcurrentTS;
		f.close();

		alljson["sections"] = {
			ModernishSection,
			CCSection,
			TrickstersSection
		};

		*outJson = std::move(alljson);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

static bool interperetFaqJson(json* outJson) {
	std::ifstream f("ChangeFaqData/mainFAQ.json");
	try
	{
		json allJson;

		json faqSection = {
			{"heading","FAQ"},
			{"subheading", "CasuallyCorporate/Surviving-Maps-CC-Modernish"}
		};
		json tmpjson = json::parse(f);
		faqSection["contents"] = tmpjson;

		allJson["sections"] = json::array({ faqSection });
		*outJson = std::move(allJson);
	}
	catch (...)
	{
		return false;
	}
	return true;
}