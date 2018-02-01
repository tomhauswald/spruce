#include "Common.h"
#include "Log.h"

namespace spruce {
	void panicIf(bool expression, std::string const& message) {
		if (expression) {
			Log::err << message;
			assert(false);
		}
	}
}