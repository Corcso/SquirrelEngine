#include "PCH.h"

#ifdef VULKAN



#include "VulkanDescriptor.h"
#include "VulkanUtility.h"
namespace SQ {
	void VulkanDescriptor::CreateAndAllocateBuffer(size_t size)
	{
		VulkanUtility::CreateBufferAndAssignMemory(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&descriptorBuffer, &descriptorBufferMemory);

		mappedMemoryLocation = VulkanUtility::OpenMemoryMap(descriptorBufferMemory, size);

		bufferSize = size;
	}

	void VulkanDescriptor::CreateDescriptorSet(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool descriptorPool)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
			throw -1;
		}

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = descriptorBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = bufferSize;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSet;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional
		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}

	VulkanDescriptor::~VulkanDescriptor()
	{
		// Close map 
		// TODO
		// and free memory
		VulkanUtility::DestroyBuffer(descriptorBuffer);
		VulkanUtility::FreeGPUMemory(descriptorBufferMemory);
	}

}
#endif // VULKAN
