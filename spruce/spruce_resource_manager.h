#pragma once

#include <map>
#include <string>
#include "spruce_common.h"

namespace spruce {

	template<class T>
	class ResourceManager {
	protected:
		static std::map<std::string, std::unique_ptr<T>> sContainer;

	public:
		static T& add(std::string const& key, std::unique_ptr<T> item) {
			return *(sContainer[key] = std::move(item));
		}

		static T *get(std::string const& key) {
			return sContainer[key].get();
		}

		static bool exists(std::string const& key) {
			return sContainer.size(key) > 0;
		}
	};

	template<class T>
	std::map<std::string, std::unique_ptr<T>> ResourceManager<T>::sContainer;
}