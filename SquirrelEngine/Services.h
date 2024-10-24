#pragma once
#include "PCH.h"
#include "Graphics.h"


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
		/// Registers the graphics service with the service locator. 
		/// Does nothing if a graphics service is already registered. 
		/// </summary>
		/// <param name="graphicsServicePointer"></param>
		static void RegisterGraphics(Graphics* graphicsServicePointer) {
			if (graphicsService.get() == nullptr) graphicsService.reset(graphicsServicePointer);
		}
	private:
		static std::unique_ptr<Graphics> graphicsService;
	};
}
