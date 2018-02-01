#pragma once

#include "Common.h"
#include <sstream>

namespace spruce {

	class Log {
	private:
		std::stringstream mHistory;
		std::stringstream mCurrent;
		std::vector<std::function<void(std::string const&)>> mCallbacks;

	public:
		static Log msg; // Message log.
		static Log err; // Error log.

		void registerCallback(std::function<void(std::string const&)> callback);

		template<class Serializable>
		Log& operator<<(Serializable const& serializable) {

			mHistory << serializable;
			mCurrent.str("");
			mCurrent << serializable;

			for (auto const& callback : mCallbacks) {
				callback(mCurrent.str());
			}

			return *this;
		}

		template<class... Args>
		Log& printf(std::string const& format, Args... args) {
			static char buf[2048];
			sprintf_s(buf, format.c_str(), args...);
			return this->operator<<(buf);
		}

		inline auto const& history() const { return mHistory.str(); }
	};
}