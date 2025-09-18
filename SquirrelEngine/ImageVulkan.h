#pragma once
#ifdef VULKAN
#include "Image.h"
#include "SquirrelEnginePI.h"
namespace SQ {
    class ImageVulkan :
        public Image
    {
    public:
        static ImageVulkan* Load(std::string path);
    private:

        VkImage image;
        VulkanMemoryAllocator::VulkanMemoryBlock imageMemory;
    };
}
#endif //VULKAN