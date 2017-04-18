#pragma once

#include <map>
#include <string>
#include "spruce_common.h"

namespace spruce {

	template<class T>
	class Resource_Manager {
	protected:
		static std::map<std::string, std::unique_ptr<T>> container_;

	public:
		static void add(std::string const& key, std::unique_ptr<T> item) {
			container_[key] = std::move(item);
		}

		static T *get(std::string const& key) {
			return container_[key].get();
		}

		static bool exists(std::string const& key) {
			return container_.size(key) > 0;
		}
	};

	template<class T>
	std::map<std::string, std::unique_ptr<T>> Resource_Manager<T>::container_;
}