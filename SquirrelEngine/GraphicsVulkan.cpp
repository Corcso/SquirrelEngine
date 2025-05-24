#include "PCH.h"
#ifdef VULKAN

#include "GraphicsVulkan.h"
#include "VulkanSetup.h"

#include "InputWindows.h"

// For HD mouse movement (Microsoft, 2023)
#include <hidusage.h>

#include "SQUtility.h"

int SQ::GraphicsVulkan::Init(std::string title, int width, int height, Vec4 clearColor)
{
    // Begin Windows Window Setup

        // Define and register window class with OS
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &InputWindows::WndProc;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassEx(&windowClass)) {
        throw 2;
    }

    // Setup window

    // Client rect is the size of the renderable area (entire window)
    RECT clientRect = { 0, 0, width, height };
    // Window rect is the client rect asjusted for the top bar
    RECT windowRect = clientRect;
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    window = CreateWindowW(WINDOW_CLASS_NAME, NStringToWString(title).c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, GetModuleHandle(NULL), nullptr);

    if (!window) {
        throw 3;
    }

    ShowWindow(window, SW_NORMAL);
    UpdateWindow(window);

    //---------------------
    // Setup raw input for HD mouse movement (Microsoft, 2023)
    // https://learn.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
    #ifndef HID_USAGE_PAGE_GENERIC
    #define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
    #endif
    #ifndef HID_USAGE_GENERIC_MOUSE
    #define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
    #endif

    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = window;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
    //---------------------

    // Begin Vulkan Setup

    // >>> Create instance
    VulkanSetup::CreateInstance(&instance);

    // >>> Create surface
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; // On windows
    surfaceCreateInfo.hwnd = window; // Our window
    surfaceCreateInfo.hinstance = GetModuleHandle(nullptr); // Hinstance

    if (vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, nullptr, &surface) != VK_SUCCESS) {
        throw -1;
    }

    // >>> Pick Device for rendering
    // Get and check device count
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    // Get all device handles
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // Check each device and pick one
    uint16_t highestPriority = 0;
    for (const auto& device : devices) {
        if (VulkanSetup::CheckDeviceSuitability(device, surface) && VulkanSetup::GetDevicePriority(device, surface) > highestPriority) {
            physicalDevice = device;
        }
    }

    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    // >>> Create logical device & queues
    // Device queue
    VulkanSetup::QueueFamilyIndices indices = VulkanSetup::GetQueueFamilyIndices(physicalDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::vector<uint32_t> uniqueQueueFamilies;
    if (indices.graphicsFamily == indices.presentFamily) uniqueQueueFamilies = { indices.graphicsFamily };
    else uniqueQueueFamilies = { indices.graphicsFamily, indices.presentFamily };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Coming back later here
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Device info
    VkDeviceCreateInfo logicDeviceCreateInfo{};
    logicDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    logicDeviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    logicDeviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();

    logicDeviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    logicDeviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(VulkanSetup::deviceExtensions.size());
    std::vector<const char*> deviceExtensionsAsCStr;
    for (const auto& string : VulkanSetup::deviceExtensions) deviceExtensionsAsCStr.push_back(string.c_str());
    logicDeviceCreateInfo.ppEnabledExtensionNames = deviceExtensionsAsCStr.data();

    // Not required only for backwards compat

    /*if (enableValidationLayers) {
        logicDeviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        logicDeviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        logicDeviceCreateInfo.enabledLayerCount = 0;
    }*/

    if (vkCreateDevice(physicalDevice, &logicDeviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
        throw -1;
    }
    // Get Queues
    vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);

    // >>> Create swap chain
    VulkanSetup::CreateSwapChain(device, physicalDevice, surface, currentWidth, currentHeight, &swapChainImageFormat, &swapChainExtent, &swapChainImages, &swapChain);
    VulkanSetup::CreateImageViewsForSwapChain(device, swapChainImageFormat, swapChainImages, &swapChainImageViews);
    
    // Setup render pass
    VulkanSetup::CreateRenderPass(device, physicalDevice, swapChainImageFormat, &renderPass);

    // Setup pipeline
    VulkanSetup::CreateGraphicsPipeline(device, renderPass, swapChainExtent, &pipelineLayout, &graphicsPipeline);

    // Setup depth buffer
    VulkanSetup::CreateDepthBuffer(device, physicalDevice, swapChainExtent, &depthImage, &depthImageMemory, &depthImageView);
       
    // Setup frame buffers
    VulkanSetup::CreateFrameBuffers(device, renderPass, swapChainExtent, swapChainImageViews, depthImageView, &swapChainFramebuffers);

    // Setup command pool & buffers
    VulkanSetup::CreateCommandPool(device, physicalDevice, surface, &commandPool);
    VulkanSetup::CreateCommandBuffers(device, commandPool, &commandBuffers);

    // Create Sync objects
    VulkanSetup::CreateSyncObjects(device, &inFlightFences, &imageAvailableSemaphores, &renderFinishedSemaphores);

    return 0;
}

void SQ::GraphicsVulkan::BeginRender()
{
   
}

void SQ::GraphicsVulkan::UpdateProjectionMatrix(CameraNut* camera)
{
}

void SQ::GraphicsVulkan::SetupCameraForFrame(CameraNut* camera)
{
}

void SQ::GraphicsVulkan::Render(MeshNut* toRender)
{
}

void SQ::GraphicsVulkan::EndRender()
{
}

void SQ::GraphicsVulkan::RegisterLightForFrame(LightNut* light)
{
}

void SQ::GraphicsVulkan::ClearFrameLights()
{
}

SQ::Vec2 SQ::GraphicsVulkan::GetRenderWindowSize()
{
    return Vec2();
}

SQ::Vec2 SQ::GraphicsVulkan::GetWindowLocation()
{
    return Vec2();
}

#endif