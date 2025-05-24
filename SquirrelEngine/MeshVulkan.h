#pragma once

#ifdef VULKAN

#include "Mesh.h"

#include "SquirrelEnginePI.h"

namespace SQ {
	class MeshVulkan : public Mesh
	{
	public:
		static VkVertexInputBindingDescription vertexBindingDescription;
		static VkVertexInputAttributeDescription* vertexAttributeDescriptions;
		static uint32_t vertexAttributeDescriptionCount;
		static void SetupBindingAttributeDescriptions();
	};
}

#endif