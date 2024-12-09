#pragma once

#include <string>
#include <map>
#include <mutex>

namespace Errors {
	const enum ToFrom {
		None,
		//Generics
		Create,
		Read,
		Update,
		Delete
	};
	const std::map<ToFrom, std::string> errorToFromtoString{
		{ ToFrom::None, "None" },
		//Generics
		{ ToFrom::Create, "Create" },
		{ ToFrom::Read, "Read" },
		{ ToFrom::Update, "Update" },
		{ ToFrom::Delete, "Delete" }
	};
	const struct ErrorMessage {
		std::string error;
		ToFrom to;
		ToFrom from;
	};
}

class IErrors {
private:
	std::mutex _errorMtx;
	Errors::ErrorMessage _errorMessage = {"", Errors::ToFrom::None, Errors::ToFrom::None };

public:
	Errors::ErrorMessage getErrorMessage() {
		std::lock_guard lock(_errorMtx);
		return _errorMessage;
	}
	void setErrorMessage(Errors::ErrorMessage error) {
		std::lock_guard lock(_errorMtx);
		_errorMessage = error;
	}
	bool hasErrored() {
		std::lock_guard lock(_errorMtx);
		return (_errorMessage.error == "");
	}
	virtual Errors::ErrorMessage getErrorPublic() = 0;
	virtual bool hasErroredPublic() = 0;
};