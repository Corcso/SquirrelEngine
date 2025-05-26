#include "PCH.h"
#include "MeshVulkan.h"

#ifdef VULKAN
#include "VulkanUtility.h"

// Setup statics
VkVertexInputBindingDescription SQ::MeshVulkan::vertexBindingDescription = {};
VkVertexInputAttributeDescription* SQ::MeshVulkan::vertexAttributeDescriptions = nullptr;
uint32_t SQ::MeshVulkan::vertexAttributeDescriptionCount = 0;

void SQ::MeshVulkan::SetupBindingAttributeDescriptions()
{
    vertexAttributeDescriptions = new VkVertexInputAttributeDescription[5];
    vertexAttributeDescriptionCount = 5;

    vertexAttributeDescriptions[0].binding = 0;
    vertexAttributeDescriptions[0].location = 0;
    vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDescriptions[0].offset = offsetof(SQ::Vertex, position);

    vertexAttributeDescriptions[1].binding = 0;
    vertexAttributeDescriptions[1].location = 1;
    vertexAttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDescriptions[1].offset = offsetof(SQ::Vertex, normal);

    vertexAttributeDescriptions[2].binding = 0;
    vertexAttributeDescriptions[2].location = 2;
    vertexAttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    vertexAttributeDescriptions[2].offset = offsetof(SQ::Vertex, textureCoordinate);

    vertexAttributeDescriptions[3].binding = 0;
    vertexAttributeDescriptions[3].location = 3;
    vertexAttributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDescriptions[3].offset = offsetof(SQ::Vertex, tangent);

    vertexAttributeDescriptions[4].binding = 0;
    vertexAttributeDescriptions[4].location = 4;
    vertexAttributeDescriptions[4].format = VK_FORMAT_R32G32B32_SFLOAT;
    vertexAttributeDescriptions[4].offset = offsetof(SQ::Vertex, bitangent);

    vertexBindingDescription = {};
    vertexBindingDescription.binding = 0;
    vertexBindingDescription.stride = sizeof(Vertex);
    vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
}

void SQ::MeshVulkan::LoadBuffers()
{
    // VERTEX BUFFER
    // Create buffer
    // Staging Vertex buffer
    VkBuffer stagingVertexBuffer;
    VkDeviceMemory stagingVertexBufferMemory;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    VulkanUtility::CreateBufferAndAssignMemory(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingVertexBuffer, &stagingVertexBufferMemory);

    // Map GPU memory to CPU memory
    VulkanUtility::MapCopyToGPU(stagingVertexBufferMemory, vertices.data(), bufferSize);

    VulkanUtility::CreateBufferAndAssignMemory(bufferSize,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        &vertexBuffer, &vertexBufferMemory);

    // See below
    VulkanUtility::CopyBufferData(stagingVertexBuffer, vertexBuffer, bufferSize);

    // Dont need the stager anymore
    VulkanUtility::DestroyBuffer(stagingVertexBuffer);
    VulkanUtility::FreeGPUMemory(stagingVertexBufferMemory);

    // INDEX BUFFER
    bufferSize = sizeof(indicies[0]) * indicies.size();

    VkBuffer stagingIndexBuffer;
    VkDeviceMemory stagingIndexBufferMemory;
    VulkanUtility::CreateBufferAndAssignMemory(bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        &stagingIndexBuffer, &stagingIndexBufferMemory);

    VulkanUtility::MapCopyToGPU(stagingIndexBufferMemory, indicies.data(), bufferSize);

    VulkanUtility::CreateBufferAndAssignMemory(bufferSize, 
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        &indexBuffer, &indexBufferMemory);

    VulkanUtility::CopyBufferData(stagingIndexBuffer, indexBuffer, bufferSize);

    VulkanUtility::DestroyBuffer(stagingIndexBuffer);
    VulkanUtility::FreeGPUMemory(stagingIndexBufferMemory);
}

#endif


