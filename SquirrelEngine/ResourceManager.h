#pragma once
#include <unordered_map>
#include "PCH.h"
#include "Resource.h"
namespace SQ {
	class ResourceManager
	{
	public:

		template<typename T>
		inline std::shared_ptr<T> Retrieve(std::string path) {
			bool loaded = false;
			if (resources.find(path) != resources.end()) {
				if (!resources[path].expired()) {
					return std::dynamic_pointer_cast<T>(resources[path].lock());
					loaded = true;
				}
			}
			if (!loaded) {
				std::shared_ptr<Resource> newlyLoadedResource = std::shared_ptr<Resource>(T::Load(path));
				resources[path] = newlyLoadedResource;
				return std::dynamic_pointer_cast<T>(newlyLoadedResource);
			}
		}

	private:
		std::unordered_map<std::string, std::weak_ptr<Resource>> resources;
	};
}