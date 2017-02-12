#include "spruce_common.h"
#include "spruce_log.h"

namespace spruce {
	void panic_if(bool expression, std::string const& message) {
		if (expression) {
			Log::err << message;
			assert(false);
		}
	}
}