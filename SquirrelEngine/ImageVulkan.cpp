#include "PCH.h"
#include "ImageVulkan.h"
#ifdef VULKAN
#include "VulkanUtility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace SQ {
    ImageVulkan* SQ::ImageVulkan::Load(std::string path)
    {
        ImageVulkan* toWorkOn = new ImageVulkan();

        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;
        
        // Failed to load image
        if (!pixels) {
            throw -1;
        }

        // Open a staging buffer with all the image data, and map it to VRAM
        VkBuffer stagingBuffer;
        VulkanMemoryAllocator::VulkanMemoryBlock stagingBufferMemory;

        VulkanUtility::CreateBufferAndAssignMemory(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory, VulkanMemoryAllocator::VulkanMemoryMapUsage::INSTANT);

        VulkanUtility::MapCopyBlockToGPU(stagingBufferMemory, pixels, imageSize);

        // Free the image data from the CPU
        stbi_image_free(pixels);

        // Transfer image to VkImage
        VulkanUtility::CreateImageAndAssignMemory(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &(toWorkOn->image), &(toWorkOn->imageMemory));

        // Transfer
        VulkanUtility::TransitionImageLayout(toWorkOn->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        // Copy
        VulkanUtility::CopyBufferToImage(stagingBuffer, toWorkOn->image, texWidth, texHeight);
        // Transfer to use
        VulkanUtility::TransitionImageLayout(toWorkOn->image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // Cleanup buffers
        VulkanUtility::DestroyBuffer(stagingBuffer);
        VulkanUtility::FreeGPUMemoryBlock(stagingBufferMemory);

        return nullptr;
    }
    ImageVulkan::~ImageVulkan()
    {
        VulkanUtility::DestroyImage(image);
        VulkanUtility::FreeGPUMemoryBlock(imageMemory);
    }
}
#endif // VULKAN