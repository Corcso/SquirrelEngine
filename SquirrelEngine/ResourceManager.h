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

		void ImGuiRenderDebugInfo();

	private:
		std::unordered_map<std::string, std::weak_ptr<Resource>> resources;
		std::mutex loaderLock;
	};
}