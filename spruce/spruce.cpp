#include "spruce.h"
#include "spruce_log.h"

namespace spruce {
	void panic_if(bool expression, std::string const& message) {
		if (expression) {
			log::err << message << newl;
			assert(false);
		}
	}
}