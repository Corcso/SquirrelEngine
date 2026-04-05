#pragma once
#include <unordered_map>
#include "PCH.h"
#include "Resource.h"
#include <mutex>
namespace SQ {
	class ResourceManager
	{
	public:

		/// <summary>
		/// Retrieve a resource based on its file path. 
		/// Loads it in if required. 
		/// </summary>
		/// <typeparam name="T">Type of resource expected</typeparam>
		/// <param name="path">Filepath of resource</param>
		/// <returns>Shared pointer to resource</returns>
		template<typename T>
		inline std::shared_ptr<T> Retrieve(std::string path) {
			// Look for the resource in the map of loaded resources. 
			bool loaded = false;
			// If found
			if (resources.find(path) != resources.end()) {
				// And loaded
				if (!resources[path].expired()) {
					// Return
					return std::dynamic_pointer_cast<T, SQ::Resource>(resources[path].lock());
					loaded = true;
				}
			}
			// If not found, or was unloaded, enter the mutex lock for loading (only 1 load at a time)
			std::lock_guard<std::mutex> lockGuard(loaderLock);
			// Once in, check again, just incase we were being blocked by something else loading the resource we want. 
			// This stops it being loaded twice. 
			// If the resource expires between the return and if statement (due to multithreading) not certain how to handle such. 
			if (resources.find(path) != resources.end()) {
				if (!resources[path].expired()) {
					return std::dynamic_pointer_cast<T, SQ::Resource>(resources[path].lock());
					loaded = true;
				}
			}
			// Its definietly not loaded, load it. 
			if (!loaded) {
				std::shared_ptr<Resource> newlyLoadedResource(T::Load(path));
				resources[path] = newlyLoadedResource;
				return std::dynamic_pointer_cast<T, SQ::Resource>(newlyLoadedResource);
			}
		}

		/// <summary>
		/// Reloads an already loaded resource from file.
		/// If the resource path isn't loaded, do nothing.
		/// </summary>
		/// <typeparam name="T">Type of resource to reload</typeparam>
		/// <param name="path">Path of resource to reload</param>
		template<typename T>
		inline void Reload(std::string path) {
			// Look for the resource in the map of loaded resources. 
			bool loaded = false;
			// If found
			if (resources.find(path) != resources.end()) {
				// And loaded
				if (!resources[path].expired()) {
					// Swap the data for the old resource to the new resources data. 
					// NOT 100% CERTAIN IF THIS WORKS OR IS SAFE
					Resource* newlyLoadedResource(T::Load(path));
					Resource oldResourceData = *resources[path].get();

					*resources[path].get() = *resources[path].get();

					// Old resource will be deconstructed as it falls out of scope, cleaning any memory it owned, but the pointer address memory is still open as that is now newResource
					// Newly loaded resource now exists inside the location of the old one, it should not be deconstructed (as that will break the kept version) but should be freed, so just use free do not use delete.
					free(newlyLoadedResource);
				}
			}
			// If its not loaded do nothing
		}

		/// <summary>
		/// Returns the path when given a loaded resource. 
		/// </summary>
		/// <param name="target">The loaded resource</param>
		/// <returns>File path of loaded resource, "" if not found.</returns>
		std::string GetPath(std::shared_ptr<Resource> target) {
			for (auto pair : resources) {
				std::shared_ptr<Resource> thisItemLock = pair.second.lock();
				if (target.get() == thisItemLock.get()) return pair.first;
			}
			return "";
		}

		void ImGuiRenderDebugInfo();

	private:
		std::unordered_map<std::string, std::weak_ptr<Resource>> resources;
		std::mutex loaderLock;
	};
}