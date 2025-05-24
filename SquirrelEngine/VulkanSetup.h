#pragma once

#ifdef VULKAN
#include "SquirrelEnginePI.h"

namespace SQ {
	class VulkanSetup
	{
	public:
		static void CreateInstance(VkInstance* instance);

		static void CreateSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t width, uint32_t height,
			VkFormat* swapChainFormat, VkExtent2D* swapChainExtent, std::vector<VkImage>* swapChainImages, VkSwapchainKHR* swapChain);

		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

		static void CreateImageViewsForSwapChain(VkDevice device, VkFormat swapChainFormat,
			const std::vector<VkImage>& swapChainImages, std::vector<VkImageView>* swapChainImageViews);

		static void CreateRenderPass(VkDevice device, VkPhysicalDevice physicalDevice, VkFormat swapChainFormat, VkRenderPass* renderPass);
		static VkFormat GetDepthBufferFormat(VkPhysicalDevice physicalDevice);

		static void CreateGraphicsPipeline(VkDevice device, VkRenderPass renderPass, VkExtent2D swapChainExtent,
			VkPipelineLayout* pipelineLayout, VkPipeline* graphicsPipeline);

		static void CreateDepthBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkExtent2D swapChainExtent, VkImage* depthImage, VkDeviceMemory* depthImageMemory, VkImageView* depthImageView);

		/// <summary>
		/// Checks if a device is suitable to render a squirrel engine game.
		/// </summary>
		static bool CheckDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface);

		/// <summary>
		/// Returns that devices priority, the highest priority device should be chosen for rendering.
		/// </summary>
		/// <param name="device">Device to check</param>
		/// <param name="surface">Surface for presenting</param>
		/// <returns>Priority 1 = lowest, ascending order.</returns>
		static uint16_t GetDevicePriority(VkPhysicalDevice device, VkSurfaceKHR surface);

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		/// <summary>
		/// Gets details of the swap chain from the passed in device. 
		/// </summary>
		/// <param name="device"></param>
		/// <returns></returns>
		static SwapChainSupportDetails GetSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

		/// <summary>
		/// Struct for storing the indices of the graphics and present queues. 
		/// </summary>
		struct QueueFamilyIndices {
			bool hasGraphicsFamily;
			uint32_t graphicsFamily;
			bool hasPresentFamily;
			uint32_t presentFamily;

			bool IsComplete() {
				return hasGraphicsFamily && hasPresentFamily;
			}
		};

		static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);

		static const std::vector<std::string> deviceExtensions;
	};
}

#endif