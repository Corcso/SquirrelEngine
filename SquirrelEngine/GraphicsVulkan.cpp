#include "PCH.h"
#ifdef VULKAN

#include "GraphicsVulkan.h"
#include "VulkanSetup.h"
#include "MeshVulkan.h"

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

    // Setup descriptor pool
    VulkanSetup::CreateDescriptorPool(device, VULKAN_MAX_FRAMES_IN_FLIGHT, VULKAN_MAX_FRAMES_IN_FLIGHT, &descriptorPool);

    SetupDescriptorSets();

    // Setup pipeline
    std::vector<VkDescriptorSetLayout> allDescriptorSetLayouts = {
        projectionViewWorldDescriptorSetLayout
    };
    VulkanSetup::CreateGraphicsPipeline(device, renderPass, swapChainExtent, allDescriptorSetLayouts, &pipelineLayout, &graphicsPipeline);

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
    // Wait until previous frame is finished. 
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Get image from swap chain
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &thisRenderImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        //recreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw -1;
    }

    // Reset it only if we know we can draw
    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    // Reset command buffer
    vkResetCommandBuffer(commandBuffers[currentFrame], 0);

    // Record command buffer setup
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
        throw -1;
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[thisRenderImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::vector<VkClearValue> clearColors = { {{0.0f, 0.0f, 0.0f, 1.0f}}, {1.0f, 0} };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
    renderPassInfo.pClearValues = clearColors.data();

    vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChainExtent.width);
    viewport.height = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);
}

void SQ::GraphicsVulkan::UpdateProjectionMatrix(CameraNut* camera)
{
    projectionViewWorldData.projection = Perspective_RH_ZO(camera->GetFov(), 1, 0.01, 200);
}

void SQ::GraphicsVulkan::SetupCameraForFrame(CameraNut* camera)
{
    projectionViewWorldData.view = camera->GetViewMatrix();
}

void SQ::GraphicsVulkan::Render(MeshNut* toRender)
{
    SQ::MeshVulkan* vulkanToRender = dynamic_cast<SQ::MeshVulkan*>(toRender->GetMesh().get());

    VkBuffer vertexBuffers[] = { vulkanToRender->GetVertexBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffers[currentFrame], vulkanToRender->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

    projectionViewWorldData.world = toRender->GetGlobalSRTWorldMatrix();
    memcpy(projectionViewWorldDescriptorSets[currentFrame].GetMappedMemoryLocation(), &projectionViewWorldData, sizeof(ProjectionViewWorldUBO));
    vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, projectionViewWorldDescriptorSets[currentFrame].GetDescriptorSet(), 0, nullptr);

    vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(vulkanToRender->GetIndexCount()), 1, 0, 0, 0);
}

void SQ::GraphicsVulkan::EndRender()
{
    // Finish recording command buffer
    vkCmdEndRenderPass(commandBuffers[currentFrame]);

    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw -1;
    }

    // Now we need to submit it
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT }; // We only care about colour writing, this allows pre rasteriser to get head start
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw -1;
    }

    // Now we present
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &thisRenderImageIndex;

    presentInfo.pResults = nullptr; // Optional

    VkResult result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR /* || framebufferResized*/) {
        //framebufferResized = false;
        //if (!toQuit) recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw -1;
    }

    currentFrame = (currentFrame + 1) % VULKAN_MAX_FRAMES_IN_FLIGHT;
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

void SQ::GraphicsVulkan::SetupDescriptorSets()
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    // Only using this in vertex shader
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    // Not used for images
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &projectionViewWorldDescriptorSetLayout) != VK_SUCCESS) {
        throw -1;
    }

    for (int i = 0; i < VULKAN_MAX_FRAMES_IN_FLIGHT; i++) {
        projectionViewWorldDescriptorSets.push_back(VulkanDescriptor());
        projectionViewWorldDescriptorSets[i].CreateAndAllocateBuffer(sizeof(ProjectionViewWorldUBO));
        projectionViewWorldDescriptorSets[i].CreateDescriptorSet(device, projectionViewWorldDescriptorSetLayout, descriptorPool);
    }
}

#endif