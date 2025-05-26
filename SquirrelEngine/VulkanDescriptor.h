#pragma once

#ifdef VULKAN
#include "SquirrelEnginePI.h"

namespace SQ {
	class VulkanDescriptor
	{
	public:
		void CreateAndAllocateBuffer(size_t size);

		void CreateDescriptorSet(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool descriptorPool);

		VkDescriptorSet* GetDescriptorSet() { return &descriptorSet; }

		void* GetMappedMemoryLocation() { return mappedMemoryLocation; }
		
		~VulkanDescriptor();
	private:
		VkDescriptorSet descriptorSet;
		VkBuffer descriptorBuffer;
		VkDeviceMemory descriptorBufferMemory;
		void* mappedMemoryLocation;
		size_t bufferSize;
	};
}
#endif // VULKAN
