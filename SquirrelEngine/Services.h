#pragma once
#include "PCH.h"
#include "Graphics.h"
#include "Input.h"
#include "Tree.h"
#include "ResourceManager.h"


namespace SQ {
	/// <summary>
	/// Service Locator 
	/// Adapted from (Nystrom, 2014)
	/// </summary>
	class Services
	{
	public:
		/// <summary>
		/// Retrieves the graphics service. 
		/// </summary>
		/// <returns>A pointer to the graphics service.</returns>
		static Graphics* GetGraphics() {
			return graphicsService.get();
		}

		/// <summary>
		/// Retrieves the input service. 
		/// </summary>
		/// <returns>A pointer to the input service.</returns>
		static Input* GetInput() {
			return inputService.get();
		}

		/// <summary>
		/// Retrieves the resource manager service. 
		/// </summary>
		/// <returns>A pointer to the resource manager service.</returns>
		static ResourceManager* GetResourceManager() {
			return resourceManagerService.get();
		}

		/// <summary>
		/// Retrieves the tree service. 
		/// </summary>
		/// <returns>A pointer to the tree service.</returns>
		static Tree* GetTree() {
			return treeService.get();
		}

		/// <summary>
		/// Registers the graphics service with the service locator. 
		/// Does nothing if a graphics service is already registered. 
		/// </summary>
		/// <param name="graphicsServicePointer"></param>
		static void RegisterGraphics(Graphics* graphicsServicePointer) {
			if (graphicsService.get() == nullptr) graphicsService.reset(graphicsServicePointer);
		}

		/// <summary>
		/// Registers the input service with the service locator.
		/// Does nothing if a input service is already registered.
		/// </summary>
		/// <param name="inputServicePointer"></param>
		static void RegisterInput(Input* inputServicePointer) {
			if (inputService.get() == nullptr) inputService.reset(inputServicePointer);
		}

		/// <summary>
		/// Registers the resource manager service with the service locator.
		/// Does nothing if a resource manager service is already registered.
		/// </summary>
		/// <param name="resourceManagerServicePointer"></param>
		static void RegisterResourceManager(ResourceManager* resourceManagerServicePointer) {
			if (resourceManagerService.get() == nullptr) resourceManagerService.reset(resourceManagerServicePointer);
		}

		/// <summary>
		/// Registers the tree service with the service locator.
		/// Does nothing if a tree service is already registered.
		/// </summary>
		/// <param name="treeServicePointer"></param>
		static void RegisterTree(Tree* treeServicePointer) {
			if (treeService.get() == nullptr) treeService.reset(treeServicePointer);
		}

	private:
		static std::unique_ptr<Graphics> graphicsService;
		static std::unique_ptr<Input> inputService;
		static std::unique_ptr<ResourceManager> resourceManagerService;
		static std::unique_ptr<Tree> treeService;
	};
}
