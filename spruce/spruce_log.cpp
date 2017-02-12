#include "spruce_log.h"

namespace spruce {

	Log Log::msg;
	Log Log::err;

	void Log::register_callback(std::function<void(std::string const&)> callback) {
		callbacks_.push_back(callback);
	}
}