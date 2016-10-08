#pragma once

#include "spruce.h"
#include <sstream>

namespace spruce {

	class log {
	private:
		std::stringstream history_;
		std::stringstream current_;
		std::vector<std::function<void(std::string const&)>> callbacks_;

	public:
		static log msg; // Message log.
		static log err; // Error log.

		void register_callback(std::function<void(std::string const&)> callback);

		template<class Serializable>
		log& operator<<(Serializable const& serializable) {

			history_ << serializable;
			current_.str("");
			current_ << serializable;

			for (auto const& callback : callbacks_) {
				callback(current_.str());
			}

			return *this;
		}

		inline auto const& history() const { return history_.str(); }
	};
}