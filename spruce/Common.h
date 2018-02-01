#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <cassert>
#include <limits>
#include <cmath>

namespace spruce {
	void panicIf(bool expression, std::string const& message);
	inline void panic(std::string const& message) { panicIf(true, message); }

	template<class... Args>
	std::string mprintf(std::string const& format, Args... args) {
		static char buf[2048];
		sprintf_s(buf, format.c_str(), args...);
		return std::string(buf);
	}
}