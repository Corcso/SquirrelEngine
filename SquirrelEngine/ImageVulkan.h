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

        virtual ~ImageVulkan() override;
    private:

        VkImage image;
        VulkanMemoryAllocator::VulkanMemoryBlock imageMemory;
    };
}
#endif //VULKAN