#pragma once

#ifdef VULKAN
#include "SquirrelEnginePI.h"
#include <fstream>
namespace SQ {
	class VulkanUtility
	{
	public:
		static VkShaderModule CreateShaderModule(VkDevice device, const std::vector<char>& code);

        static uint32_t FindMemoryTypeIndex(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		static inline std::vector<char> ReadFile(const std::string& filename) {
            std::ifstream file(filename, std::ios::ate | std::ios::binary);

            if (!file.is_open()) {
                throw -1;
            }

            size_t fileSize = (size_t)file.tellg();
            std::vector<char> buffer(fileSize);

            file.seekg(0);
            file.read(buffer.data(), fileSize);

            file.close();

            return buffer;

        }
	};
}

#endif // VULKAN