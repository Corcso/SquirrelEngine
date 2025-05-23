#pragma once

#ifdef VULKAN
#include "SquirrelEnginePI.h"

#include "Graphics.h"

namespace SQ {
    class GraphicsVulkan :
        public Graphics
    {
    public:
        virtual int Init(std::string title, int width, int height, Vec4 clearColor) final;


        virtual void BeginRender() final;
        virtual void UpdateProjectionMatrix(CameraNut* camera) final;

        virtual void SetupCameraForFrame(CameraNut* camera) final;
        virtual void Render(MeshNut* toRender) final;
        virtual void EndRender() final;

        virtual void RegisterLightForFrame(LightNut* light) final;
        virtual void ClearFrameLights() final;

        virtual Vec2 GetRenderWindowSize() final;
        virtual Vec2 GetWindowLocation() final;
    private:
        // Window class name must be unique
        const LPCWSTR WINDOW_CLASS_NAME = L"SquirrelEngineGame";

        // Windows Window Handle
        HWND window;

        // Clear colour
        Vec4 clearColor;

        // == Vulkan Objects ==

        // Instance
        VkInstance instance;
        // Device
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        // Logical Device
        VkDevice device;
        // Graphics queue
        VkQueue graphicsQueue;
        // Present queue
        VkQueue presentQueue;
        // Window surface
        VkSurfaceKHR surface;
        // The swap chain 
        VkSwapchainKHR swapChain;
        // Images within the swap chain & their views
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFramebuffers;
        // Depth buffer, seperate from the swap chain
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        // Store the format and extent we used for the swap chain, needed later
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        // Pipeline layout
        VkPipelineLayout pipelineLayout;
        // Render pass 
        VkRenderPass renderPass;
        // The Descriptor Set Layout (For Uniform Variables)
        std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
        // Descriptor pool
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        // The pipeline
        VkPipeline graphicsPipeline;
        // Command pool & buffer
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;

        // Sync Objects
        // Semaphores are for the GPU and Fences CPU
        std::vector<VkSemaphore> imageAvailableSemaphores; // Image acquired from the swap chain, rendering ready
        std::vector<VkSemaphore> renderFinishedSemaphores; // Render complete, presentation ready.
        std::vector<VkFence> inFlightFences; // Previous frame finished. 

        // 0 or 1 depending on the frame we are drawing
        uint32_t currentFrame = 0;
    };
}

#endif