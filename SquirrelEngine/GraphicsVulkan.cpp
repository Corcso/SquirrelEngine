#include "PCH.h"
#ifdef VULKAN

#include "GraphicsVulkan.h"
#include "VulkanSetup.h"
#include "MeshVulkan.h"
#include "Services.h"
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

#ifdef SQ_EDITOR
    // Im gui init
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(window);
#endif // SQ_EDITOR

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
    VulkanSetup::CreateDescriptorPool(device, 1, VULKAN_MAX_FRAMES_IN_FLIGHT * 100, &descriptorPool);

    SetupDescriptorSets();

    // Setup pipeline
    std::vector<VkDescriptorSetLayout> allDescriptorSetLayouts = {
        perObjectSetLayout
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

    // Store clear colour
    this->clearColor = clearColor;
    
#ifdef SQ_EDITOR
    // Editor only ImGui Setup
    ImGui_ImplVulkan_InitInfo init_info = {};
    //init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = instance;
    init_info.PhysicalDevice = physicalDevice;
    init_info.Device = device;
    init_info.QueueFamily = indices.graphicsFamily;
    init_info.Queue = graphicsQueue;
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = descriptorPool;
    init_info.RenderPass = renderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;
    init_info.ImageCount = swapChainImages.size();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = VK_NULL_HANDLE;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info);

    editorViewportExtent = {800, 800};

    VulkanSetup::CreateEditorViewport(device, physicalDevice, editorViewportExtent, swapChainImageFormat, renderPass,
        &editorViewport, &editorViewportImageView, &editorViewportFrameBuffer, &editorViewportSampler, &editorViewportMemory,
        &editorDepthImage, &editorDepthImageView, &editorDepthImageMemory);
    std::cout << "FOR REF IMAGE: " << editorViewport << " VIEW: " << editorViewportImageView << "\n";
    editorViewportDescriptorSet = reinterpret_cast<ImTextureID>(ImGui_ImplVulkan_AddTexture(editorViewportSampler, editorViewportImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
#endif // SQ_EDITOR
    return 0;
}

void SQ::GraphicsVulkan::Shutdown()
{
}

void SQ::GraphicsVulkan::BeginRender()
{
    // Wait until previous frame is finished. 
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Get image from swap chain
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &thisRenderImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
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

    std::vector<VkClearValue> clearColors = { {{clearColor.R, clearColor.G, clearColor.B, clearColor.A}}, {1.0f, 0} };
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

    thisFramesDrawCall = 0;
}

void SQ::GraphicsVulkan::UpdateProjectionMatrix(CameraNut* camera)
{
    projectionViewWorldData.projection = Perspective_RH_ZO(camera->GetFov(), 1, 0.01, 200);
}

void SQ::GraphicsVulkan::SetupCameraForFrame(CameraNut* camera)
{
    projectionViewWorldData.view = camera->GetViewMatrix();
    cameraBufferData.viewMatrix = projectionViewWorldData.view;
    cameraBufferData.cameraPosition = camera->GetPosition();
}

void SQ::GraphicsVulkan::Render(MeshNut* toRender)
{
    SQ::MeshVulkan* vulkanToRender = dynamic_cast<SQ::MeshVulkan*>(toRender->GetMesh().get());

    VkBuffer vertexBuffers[] = { vulkanToRender->GetVertexBuffer() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(commandBuffers[currentFrame], vulkanToRender->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

    if (thisFramesDrawCall >= perObjectSets[currentFrame].size()) AddAdditionalDescriptorSet(perObjectSets, perObjectSetLayout);

    projectionViewWorldData.world = toRender->GetGlobalSRTWorldMatrix();
    memcpy(perObjectSets[currentFrame][thisFramesDrawCall].GetMappedMemoryLocation(0), &projectionViewWorldData, sizeof(ProjectionViewWorldUBO));
    
    memcpy(perObjectSets[currentFrame][thisFramesDrawCall].GetMappedMemoryLocation(1), &cameraBufferData, sizeof(CameraBufferData));
    //materialBufferData = static_cast<MaterialVulkan*>(toRender->GetMaterial().get())->GetBufferData();
    if (toRender->GetMaterial().get() == nullptr) { 
        memcpy(perObjectSets[currentFrame][thisFramesDrawCall].GetMappedMemoryLocation(2), &materialBufferData, sizeof(MaterialVulkan::MaterialVulkanData)); 
    }
    else {
        memcpy(perObjectSets[currentFrame][thisFramesDrawCall].GetMappedMemoryLocation(2), static_cast<MaterialVulkan*>(toRender->GetMaterial().get())->GetBufferData(), sizeof(MaterialVulkan::MaterialVulkanData));
    }
    memcpy(perObjectSets[currentFrame][thisFramesDrawCall].GetMappedMemoryLocation(3), &lightBufferData, sizeof(LightsBufferData));
    vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, perObjectSets[currentFrame][thisFramesDrawCall].GetDescriptorSet(), 0, nullptr);

    vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(vulkanToRender->GetIndexCount()), 1, 0, 0, 0);

    thisFramesDrawCall++;
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

    //std::cout << "Draws this frame: " << thisFramesDrawCall << "\n";

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

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR  || swapChainNeedsRecreation) {
        swapChainNeedsRecreation = false;
        if (!Services::GetTree()->IsGameClosingThisFrame())RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw -1;
    }

    currentFrame = (currentFrame + 1) % VULKAN_MAX_FRAMES_IN_FLIGHT;
}

void SQ::GraphicsVulkan::RegisterLightForFrame(LightNut* light)
{
    // If light count is at its max (8) do nothing
    if (lightBufferData.lightCount >= 8) return;

    // Set all the lights attributes
    lightBufferData.lights[lightBufferData.lightCount].lightPosition = light->GetGlobalPosition();
    lightBufferData.lights[lightBufferData.lightCount].lightDirection = light->GetForward();
    lightBufferData.lights[lightBufferData.lightCount].diffuseColor = light->GetDiffuseColor();
    lightBufferData.lights[lightBufferData.lightCount].ambientColor = light->GetAmbientColor();
    lightBufferData.lights[lightBufferData.lightCount].intensity = light->GetIntensity();
    lightBufferData.lights[lightBufferData.lightCount].ambientIntensity = light->GetAmbientIntensity();
    lightBufferData.lights[lightBufferData.lightCount].lightType = (unsigned int)light->GetLightType();
    lightBufferData.lights[lightBufferData.lightCount].innerCutoffAngle = light->GetSpotlightInnerAngle();
    lightBufferData.lights[lightBufferData.lightCount].outerCutoffAngle = light->GetSpotlightOuterAngle();
    lightBufferData.lights[lightBufferData.lightCount].linearAttenuation = light->GetLinearAttenuation();
    lightBufferData.lights[lightBufferData.lightCount].quadraticAttenuation = light->GetQuadraticAttenuation();

    // Increase the light count
    lightBufferData.lightCount++;
}

void SQ::GraphicsVulkan::ClearFrameLights()
{
    // Set light count to 0
    lightBufferData.lightCount = 0;
}

SQ::Vec2 SQ::GraphicsVulkan::GetRenderWindowSize()
{
    return V2(swapChainExtent.width, swapChainExtent.height);
}

SQ::Vec2 SQ::GraphicsVulkan::GetWindowLocation()
{
    RECT rect = { NULL };
    Vec2 location;
    if (GetWindowRect(window, &rect)) {
        location.X = rect.left;
        location.Y = rect.top;
    }

    return location;
}

void SQ::GraphicsVulkan::RegisterWindowSizeChange(Vec2 newSize)
{
    newSwapChainSize = newSize;
    swapChainNeedsRecreation = true;
}

void SQ::GraphicsVulkan::BeginEditorRender()
{
#ifdef SQ_EDITOR
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    // Wait until previous frame is finished. 
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    // Get image from swap chain
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &thisRenderImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        RecreateSwapChain();
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw - 1;
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
        throw - 1;
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = editorViewportFrameBuffer;//swapChainFramebuffers[thisRenderImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = editorViewportExtent;

    std::vector<VkClearValue> clearColors = { {{clearColor.R, clearColor.G, clearColor.B, clearColor.A}}, {1.0f, 0} };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
    renderPassInfo.pClearValues = clearColors.data();

    vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(editorViewportExtent.width);
    viewport.height = static_cast<float>(editorViewportExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = editorViewportExtent;
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

//    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    thisFramesDrawCall = 0;
#endif // SQ_EDITOR
}

void SQ::GraphicsVulkan::EndEditorRender()
{
#ifdef SQ_EDITOR
    // Finish recording command buffer
    vkCmdEndRenderPass(commandBuffers[currentFrame]);

    // Wait for viewport to be available for rendering
    // TODO learn more about this!
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = editorViewport;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = 0; // TODO
    barrier.dstAccessMask = 0; // TODO

    vkCmdPipelineBarrier(
        commandBuffers[currentFrame],
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT /* TODO */, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT /* TODO */,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );


    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[thisRenderImageIndex];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = swapChainExtent;

    std::vector<VkClearValue> clearColors = { {{clearColor.R, clearColor.G, clearColor.B, clearColor.A}}, {1.0f, 0} };
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearColors.size());
    renderPassInfo.pClearValues = clearColors.data();

    vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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

    

    memoryAllocator.RenderMemoryUsageStat();
    ImGui::ShowDemoWindow();
    ImGui::Begin("Viewport");

    ImGui::Image(editorViewportDescriptorSet, ImVec2(800, 800));

    ImGui::End();
    ImGui::Render();
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffers[currentFrame]);

    // Finish recording command buffer
    vkCmdEndRenderPass(commandBuffers[currentFrame]);

    if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
        throw - 1;
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
        throw - 1;
    }

    //std::cout << "Draws this frame: " << thisFramesDrawCall << "\n";

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

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || swapChainNeedsRecreation) {
        swapChainNeedsRecreation = false;
        if (!Services::GetTree()->IsGameClosingThisFrame())RecreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw - 1;
    }

    currentFrame = (currentFrame + 1) % VULKAN_MAX_FRAMES_IN_FLIGHT;
#endif // SQ_EDITOR
}

void SQ::GraphicsVulkan::RecreateSwapChain()
{
    // If we are minimised, size is 0, freeze all main thread processing appart from the input loop (as this will see when we unminimise)
    while (newSwapChainSize.Width == 0 || newSwapChainSize.Height == 0) {
        SQ::Services::GetInput()->ProcessInput();
    }
    // Wait until nothing is going on
    vkDeviceWaitIdle(device);

    // Cleanup the swapchain
    CleanupSwapChain();

    // Make a new one
    VulkanSetup::CreateSwapChain(device, physicalDevice, surface, newSwapChainSize.Width, newSwapChainSize.Height, &swapChainImageFormat, &swapChainExtent, &swapChainImages, &swapChain);
    VulkanSetup::CreateImageViewsForSwapChain(device, swapChainImageFormat, swapChainImages, &swapChainImageViews);
    VulkanSetup::CreateDepthBuffer(device, physicalDevice, swapChainExtent, &depthImage, &depthImageMemory, &depthImageView);
    VulkanSetup::CreateFrameBuffers(device, renderPass, swapChainExtent, swapChainImageViews, depthImageView, &swapChainFramebuffers);
}

void SQ::GraphicsVulkan::CleanupSwapChain()
{
    // Taken from vulkan tutorial
    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);

    for (size_t i = 0; i < swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        vkDestroyImageView(device, swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void SQ::GraphicsVulkan::SetupDescriptorSets()
{
    std::vector<VkDescriptorSetLayoutBinding> uboLayoutBindings(4);
    uboLayoutBindings[0].binding = 0;
    uboLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBindings[0].descriptorCount = 1;
    // Only using this in vertex shader
    uboLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    // Not used for images
    uboLayoutBindings[0].pImmutableSamplers = nullptr;

    uboLayoutBindings[1].binding = 1;
    uboLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBindings[1].descriptorCount = 1;
    uboLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    uboLayoutBindings[1].pImmutableSamplers = nullptr;

    uboLayoutBindings[2].binding = 2;
    uboLayoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBindings[2].descriptorCount = 1;
    uboLayoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    uboLayoutBindings[2].pImmutableSamplers = nullptr;

    uboLayoutBindings[3].binding = 3;
    uboLayoutBindings[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBindings[3].descriptorCount = 1;
    uboLayoutBindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    uboLayoutBindings[3].pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = uboLayoutBindings.size();
    layoutInfo.pBindings = uboLayoutBindings.data();

    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &perObjectSetLayout) != VK_SUCCESS) {
        throw -1;
    }

    for (int i = 0; i < VULKAN_MAX_FRAMES_IN_FLIGHT; i++) {
        perObjectSets.push_back(std::vector<VulkanDescriptor>());
        //for (int o = 0; o < 2; o++) {
        //    //projectionViewWorldDescriptorSets[i][o].push_back(VulkanDescriptor());
        //    projectionViewWorldDescriptorSets[i][o].CreateAndAllocateBuffer(sizeof(ProjectionViewWorldUBO));
        //    projectionViewWorldDescriptorSets[i][o].CreateDescriptorSet(device, projectionViewWorldDescriptorSetLayout, descriptorPool);
        //}
    }
}

void SQ::GraphicsVulkan::AddAdditionalDescriptorSet(std::vector<std::vector<VulkanDescriptor>>& descriptorSetList, const VkDescriptorSetLayout& setLayout)
{
    std::vector<size_t> sizes = { sizeof(ProjectionViewWorldUBO), sizeof(CameraBufferData), sizeof(MaterialVulkan::MaterialVulkanData), sizeof(LightsBufferData) };

    for (int i = 0; i < VULKAN_MAX_FRAMES_IN_FLIGHT; i++) {
        int newSetIndex = descriptorSetList[i].size();
        descriptorSetList[i].push_back(VulkanDescriptor());
        descriptorSetList[i][newSetIndex].CreateAndAllocateBuffers(sizes.data(), sizes.size());
        descriptorSetList[i][newSetIndex].CreateDescriptorSet(device, perObjectSetLayout, descriptorPool);
    }
}

#endif