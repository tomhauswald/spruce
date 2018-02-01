#include "Log.h"

namespace spruce {

	Log Log::msg;
	Log Log::err;

	void Log::registerCallback(std::function<void(std::string const&)> callback) {
		mCallbacks.push_back(callback);
	}
}