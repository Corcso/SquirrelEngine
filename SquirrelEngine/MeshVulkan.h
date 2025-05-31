#pragma once

#ifdef VULKAN

#include "Mesh.h"

#include "SquirrelEnginePI.h"
#include "VulkanMemoryAllocator.h"

namespace SQ {
	class MeshVulkan : public Mesh
	{
	public:
		static VkVertexInputBindingDescription vertexBindingDescription;
		static VkVertexInputAttributeDescription* vertexAttributeDescriptions;
		static uint32_t vertexAttributeDescriptionCount;
		static void SetupBindingAttributeDescriptions();

		void LoadBuffers();

		VkBuffer GetVertexBuffer() { return vertexBuffer; }
		VkBuffer GetIndexBuffer() { return indexBuffer; }
	private:
		VkBuffer vertexBuffer;
		VulkanMemoryAllocator::VulkanMemoryBlock vertexBufferMemory;
		//VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VulkanMemoryAllocator::VulkanMemoryBlock indexBufferMemory;
		//VkDeviceMemory indexBufferMemory;
	};
}

#endif