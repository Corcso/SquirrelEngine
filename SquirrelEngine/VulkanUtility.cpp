#include "PCH.h"
#include "VulkanUtility.h"

const std::vector<std::string> SQ::VulkanUtility::deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void SQ::VulkanUtility::CreateInstance(VkInstance* instance)
{
    // First of all, check if we are having validation layers and set them up
    bool enableValidationLayers = false;
#ifdef _DEBUG
    enableValidationLayers = true;

    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // Validation layers we are looking for 
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            enableValidationLayers = false;
        }
    }
#endif

    // Setup app info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Squirrel Game";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Setup instance creation info
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    // We are having 2 extensions, windows and surface
    std::vector<const char*> extensionsNames = {
        "VK_KHR_surface", "VK_KHR_win32_surface"
    };

    createInfo.enabledExtensionCount = extensionsNames.size();
    createInfo.ppEnabledExtensionNames = extensionsNames.data();

    // Validation layers layeers
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    // Create the instance
    if (vkCreateInstance(&createInfo, nullptr, instance) != VK_SUCCESS) {
        throw -1;
    }
}

void SQ::VulkanUtility::CreateSwapChain(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t width, uint32_t height,
    VkFormat* swapChainFormat, VkExtent2D* swapChainExtent, std::vector<VkImage>* swapChainImages, VkSwapchainKHR* swapChain)
{
    SwapChainSupportDetails swapChainSupport = GetSwapChainSupportDetails(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, width, height);

    // Set the image count in the swap chain (recomended 1 more than the min)
    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    // Make sure we havent exceded the max ( 0 = no max)
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }
    // Information struct
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1; // Number of layer in each image (1 unless making VR multi image apps)
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT; // Usage (currently directly render to)

    // Tell the swap chain how to handle images accross multiple queue families. 
    QueueFamilyIndices indices = GetQueueFamilyIndices(physicalDevice, surface);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }
    // Specify we dont want to transform the image in any way, like rotate it. 
    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    // Should we blend with other windows? (no)
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // Set present mode, and we dont care about obscured pixels
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    // If the window is resized the entire swap chain needs recreated. 
    // We can give the swap chain the old one, we dont have an old one this is the first one. 
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Create it
    if (vkCreateSwapchainKHR(device, &createInfo, nullptr, swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    // Get the images, we only specified a minimum so vulkan might have made more
    vkGetSwapchainImagesKHR(device, *swapChain, &imageCount, nullptr);
    swapChainImages->resize(imageCount);
    vkGetSwapchainImagesKHR(device, *swapChain, &imageCount, swapChainImages->data());

    *swapChainFormat = surfaceFormat.format;
    *swapChainExtent = extent;
}

VkSurfaceFormatKHR SQ::VulkanUtility::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    // Look for SRGB 32 Bit 
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    // Otherwise just use first one
    return availableFormats[0];
}

VkPresentModeKHR SQ::VulkanUtility::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    // Choose mailbox, tripple buffer if possible
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    // Otherwise choose queued swap chain (guaranteed availability)
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SQ::VulkanUtility::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height) {
    // Usually current extent is already set to the window size but if its max uint32 then its unknown 
    if (capabilities.currentExtent.width != 4294967295) {
        return capabilities.currentExtent;
    }
    // So if its max uint32 then find the window size in pixels.
    else {
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void SQ::VulkanUtility::CreateImageViewsForSwapChain(VkDevice device, VkFormat swapChainFormat,
    const std::vector<VkImage>& swapChainImages, std::vector<VkImageView>* swapChainImageViews)
{
    // Same number of views as images
    swapChainImageViews->resize(swapChainImages.size());

    // For each image create a view for it 
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainFormat;

        // Set default mapping for each channel
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        // Images purpose and part to be accessed
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(device, &createInfo, nullptr, &(*swapChainImageViews)[i]) != VK_SUCCESS) {
            throw -1;
        }
    }
}

bool SQ::VulkanUtility::CheckDeviceSuitability(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    // Properties, like name, types etc. 
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    // Features, 64 bit support, multi viewport rendering, etc. 
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    // === Check Queue Families ===
    QueueFamilyIndices indices = GetQueueFamilyIndices(device, surface);
    

    // === Check extension support ===
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    uint32_t extensionsFound = 0;

    for (const auto& extension : availableExtensions) {
        for(const auto& desiredExtension : deviceExtensions){
            if (desiredExtension == extension.extensionName) extensionsFound++;
        }
    }

    bool extensionsSupported = extensionsFound == deviceExtensions.size();


    // Check swap chain support
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = GetSwapChainSupportDetails(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    // We must have these things
    return indices.IsComplete() && extensionsSupported && swapChainAdequate;

}

uint16_t SQ::VulkanUtility::GetDevicePriority(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    // Properties, like name, types etc. 
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    // Simply prefer dedicated GPUs for now
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 2 : 1;
}

SQ::VulkanUtility::SwapChainSupportDetails SQ::VulkanUtility::GetSwapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails details;

    // Get surface capabilities 
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    // Get surface formats (list)
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // Get present modes (list)
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

SQ::VulkanUtility::QueueFamilyIndices SQ::VulkanUtility::GetQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    QueueFamilyIndices indices;
    // Logic to find queue family indices to populate struct with
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.hasGraphicsFamily = true;
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport) {
            indices.hasPresentFamily = true;
            indices.presentFamily = i;
        }

        if (indices.IsComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
