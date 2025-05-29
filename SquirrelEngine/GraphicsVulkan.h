#pragma once

#ifdef VULKAN
#include "SquirrelEnginePI.h"
#include "VulkanUtility.h"
#include "Graphics.h"
#include "VulkanDescriptor.h"
#include "MaterialVulkan.h"

namespace SQ {

    class GraphicsVulkan :
        public Graphics
    {
    public:
        friend class VulkanUtility;

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

        uint32_t currentWidth;
        uint32_t currentHeight;

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
        // Descriptor pool
        VkDescriptorPool descriptorPool;
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

        // This frame's draw call
        uint32_t thisFramesDrawCall = 0;

        // This render pass's image index in the swap chain
        uint32_t thisRenderImageIndex;

        // Uniform Buffer Objects

        // TODO Split world into its own UBO

        void SetupDescriptorSets();

        void AddAdditionalDescriptorSet(std::vector<std::vector<VulkanDescriptor>>& descriptorSetList, const VkDescriptorSetLayout& setLayout);

        struct ProjectionViewWorldUBO {
            alignas(16) Mat4 projection;
            alignas(16) Mat4 view;
            alignas(16) Mat4 world;
        };
        struct LightBufferData {
            alignas(16) Vec3 lightPosition;

            alignas(16) Vec3 lightDirection;

            alignas(16) Vec3 diffuseColor;

            alignas(16) Vec3 ambientColor;
            alignas(4) float intensity;

            alignas(4) float ambientIntensity;
            alignas(4) unsigned int lightType;
            alignas(4) float innerCutoffAngle;
            alignas(4) float outerCutoffAngle;

            // No need for constant attenuation, this can be edited by changing intensity. 
            alignas(4) float linearAttenuation;
            alignas(4) float quadraticAttenuation;
        };

        struct LightsBufferData {
            alignas(16) LightBufferData lights[8];
            alignas(4) unsigned int lightCount;
        };
        struct CameraBufferData {
            alignas(16) Mat4 viewMatrix;
            alignas(16) Vec3 cameraPosition;
        };
        VkDescriptorSetLayout perObjectSetLayout;
        std::vector<std::vector<VulkanDescriptor>> perObjectSets;
        ProjectionViewWorldUBO projectionViewWorldData;
        CameraBufferData cameraBufferData;
        MaterialVulkan::MaterialVulkanData materialBufferData = { {0.8, 0.8, 0.8}, {1, 1, 1}, 128, 0.5 };
        LightsBufferData lightBufferData;
    };
}

#endif