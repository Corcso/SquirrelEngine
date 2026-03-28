#pragma once
#include "PCH.h"
#include "Graphics.h"
#include "Input.h"
#include "Tree.h"
#include "ResourceManager.h"
#include "PoolAllocationService.h"
#include "SQTime.h"
#include "Physics.h"
#include "EditorService.h"


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
		/// Retrieves the pool allocation service. 
		/// </summary>
		/// <returns>A pointer to the pool allocation service.</returns>
		static PoolAllocationService* GetPoolAllocationService() {
			return poolAllocationService.get();
		}

		/// <summary>
		/// Retrieves the time service. 
		/// </summary>
		/// <returns>A pointer to the time service.</returns>
		static Time* GetTime() {
			return timeService.get();
		}

		/// <summary>
		/// Retrieves the physics service. 
		/// </summary>
		/// <returns>A pointer to the physics service.</returns>
		static Physics* GetPhysics() {
			return physicsService.get();
		}

		/// <summary>
		/// Retrieves the editor service. 
		/// </summary>
		/// <returns>A pointer to the editor service.</returns>
		static EditorService* GetEditorService() {
			return editorService.get();
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

		/// <summary>
		/// Registers the pool allocation service with the service locator.
		/// Does nothing if a pool allocation service is already registered.
		/// </summary>
		/// <param name="treeServicePointer"></param>
		static void RegisterPoolAllocationService(PoolAllocationService* poolAllocationServicePointer) {
			if (poolAllocationService.get() == nullptr) poolAllocationService.reset(poolAllocationServicePointer);
		}

		/// <summary>
		/// Registers the time service with the service locator.
		/// Does nothing if a time service is already registered.
		/// </summary>
		/// <param name="treeServicePointer"></param>
		static void RegisterTime(Time* timeServicePointer) {
			if (timeService.get() == nullptr) timeService.reset(timeServicePointer);
		}

		/// <summary>
		/// Registers the physics service with the service locator.
		/// Does nothing if a physics service is already registered.
		/// </summary>
		/// <param name="treeServicePointer"></param>
		static void RegisterPhysics(Physics* physicsServicePointer) {
			if (physicsService.get() == nullptr) physicsService.reset(physicsServicePointer);
		}

		/// <summary>
		/// Registers the editor service with the service locator.
		/// Does nothing if a editor service is already registered.
		/// </summary>
		/// <param name="editorServicePointer"></param>
		static void RegisterEditorService(EditorService* editorServicePointer) {
			if (editorService.get() == nullptr) editorService.reset(editorServicePointer);
		}

	private:
		static std::unique_ptr<Graphics> graphicsService;
		static std::unique_ptr<Input> inputService;
		static std::unique_ptr<ResourceManager> resourceManagerService;
		static std::unique_ptr<Tree> treeService;
		static std::unique_ptr<PoolAllocationService> poolAllocationService;
		static std::unique_ptr<Time> timeService;
		static std::unique_ptr<Physics> physicsService;
		static std::unique_ptr<EditorService> editorService;
	};

	// QUICK ACCESS GLOBAL FUNCTIONS

	/// <summary>
	/// Retrieves the graphics service. 
	/// </summary>
	/// <returns>A pointer to the graphics service.</returns>
	static Graphics* GetGraphics() {
		return Services::GetGraphics();
	}

	/// <summary>
	/// Retrieves the input service. 
	/// </summary>
	/// <returns>A pointer to the input service.</returns>
	static Input* GetInput() {
		return Services::GetInput();
	}

	/// <summary>
	/// Retrieves the resource manager service. 
	/// </summary>
	/// <returns>A pointer to the resource manager service.</returns>
	static ResourceManager* GetResourceManager() {
		return Services::GetResourceManager();
	}

	/// <summary>
	/// Retrieves the tree service. 
	/// </summary>
	/// <returns>A pointer to the tree service.</returns>
	static Tree* GetTree() {
		return Services::GetTree();
	}

	/// <summary>
	/// Retrieves the pool allocation service. 
	/// </summary>
	/// <returns>A pointer to the pool allocation service.</returns>
	static PoolAllocationService* GetPoolAllocationService() {
		return Services::GetPoolAllocationService();
	}

	/// <summary>
	/// Retrieves the time service. 
	/// </summary>
	/// <returns>A pointer to the time service.</returns>
	static Time* GetTime() {
		return Services::GetTime();
	}

	/// <summary>
	/// Retrieves the physics service. 
	/// </summary>
	/// <returns>A pointer to the physics service.</returns>
	static Physics* GetPhysics() {
		return Services::GetPhysics();
	}

	/// <summary>
	/// Retrieves the editor service. 
	/// </summary>
	/// <returns>A pointer to the editor service.</returns>
	static EditorService* GetEditorService() {
		return Services::GetEditorService();
	}
}
