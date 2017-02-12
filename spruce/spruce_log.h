#pragma once

#include "spruce_common.h"
#include <sstream>

namespace spruce {

	class Log {
	private:
		std::stringstream history_;
		std::stringstream current_;
		std::vector<std::function<void(std::string const&)>> callbacks_;

	public:
		static Log msg; // Message log.
		static Log err; // Error log.

		void register_callback(std::function<void(std::string const&)> callback);

		template<class Serializable>
		Log& operator<<(Serializable const& serializable) {

			history_ << serializable;
			current_.str("");
			current_ << serializable;

			for (auto const& callback : callbacks_) {
				callback(current_.str());
			}

			return *this;
		}

		template<class... Args>
		Log& printf(std::string const& format, Args... args) {
			static char buf[2048];
			sprintf_s(buf, format.c_str(), args...);
			return this->operator<<(buf);
		}

		inline auto const& history() const { return history_.str(); }
	};
}