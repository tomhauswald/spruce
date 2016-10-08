#include "spruce_log.h"

namespace spruce {

	log log::msg;
	log log::err;

	void log::register_callback(std::function<void(std::string const&)> callback) {
		callbacks_.push_back(callback);
	}
}