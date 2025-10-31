#pragma once

#include <string>
#include <memory>
#include <mutex>

class IErrors {
private:
	std::mutex _errorMtx;
	std::string _errorMessage = "";

public:
	std::string getErrorString() {
		std::lock_guard lock(_errorMtx);
		return _errorMessage;
	}
	void setErrorMessage(std::string error) {
		std::lock_guard lock(_errorMtx);
		_errorMessage = error;
	}
	bool hasErrored() {
		std::lock_guard lock(_errorMtx);
		return (_errorMessage != "");
	}
};