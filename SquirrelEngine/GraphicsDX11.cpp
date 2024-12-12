 #include "PCH.h"

#ifdef DX11
#include "GraphicsDX11.h"
#include "InputWindows.h"

// For HD mouse movement (Microsoft, 2023)
#include <hidusage.h>

// Include the compiled base shaders
#include "DX11BaseVertex_CompiledShader.h"
#include "DX11BasePixel_CompiledShader.h"

// Include required resources
#include "MaterialDX11.h"
#include "MeshDX11.h"

#include "SQUtility.h"

namespace SQ {
    int GraphicsDX11::Init(std::string title, int width, int height)
    {
        // Check for DirectX Math library support.
        if (!DirectX::XMVerifyCPUSupport())
        {
            throw 1;
        }

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

        // Begin DirectX11 Setup

        // Setup swap chain description, this is responsible for swapping the back & front buffers
        DXGI_SWAP_CHAIN_DESC swapChainDescription;
        ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC)); // Required to remove any default options.

        swapChainDescription.BufferCount = 1;
        swapChainDescription.BufferDesc.Width = width;
        swapChainDescription.BufferDesc.Height = height;
        swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDescription.BufferDesc.RefreshRate = DXGI_RATIONAL{ 0, 1 }; // Uncapped Frame Rate
        swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDescription.OutputWindow = window;
        swapChainDescription.SampleDesc.Count = 1;
        swapChainDescription.SampleDesc.Quality = 0;
        swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDescription.Windowed = TRUE;

        // Set device flags, only notable one is debug if debug build
        UINT createDeviceFlags = 0;
#if _DEBUG
        createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG; //Set flags to debug mode if debugging
#endif

        // Feature levels of direct X Supported
        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };
        // Stores the feature level actually used
        D3D_FEATURE_LEVEL featureLevel;

        // Create the device, device context and swap chain
        HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
            nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
            D3D11_SDK_VERSION, &swapChainDescription, swapChain.GetAddressOf(), device.GetAddressOf(), &featureLevel,
            deviceContext.GetAddressOf());

        // Fail and return error code if any creation fails
        if (FAILED(hr))
        {
            throw 4;
        }

        // Get the back buffer (from swap chain) and use that to create the render target view (which will render to that)
        ID3D11Texture2D* backBuffer;
        hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
        if (FAILED(hr))
        {
            throw 5;
        }

        hr = device->CreateRenderTargetView(backBuffer, nullptr, renderTargetView.GetAddressOf());
        if (FAILED(hr))
        {
            throw 6;
        }

        // Create the depth buffer for use with the depth/stencil view.
        D3D11_TEXTURE2D_DESC depthStencilBufferDescription;
        ZeroMemory(&depthStencilBufferDescription, sizeof(D3D11_TEXTURE2D_DESC));

        depthStencilBufferDescription.ArraySize = 1;
        depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilBufferDescription.CPUAccessFlags = 0; // No CPU access required.
        depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //24 depth bits, 8 stencil
        depthStencilBufferDescription.Width = width;
        depthStencilBufferDescription.Height = height;
        depthStencilBufferDescription.MipLevels = 1;
        depthStencilBufferDescription.SampleDesc.Count = 1;
        depthStencilBufferDescription.SampleDesc.Quality = 0;
        depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;

        hr = device->CreateTexture2D(&depthStencilBufferDescription, nullptr, depthStencilBuffer.GetAddressOf());
        if (FAILED(hr))
        {
            throw 7;
        }

        // Create depth stencil view
        hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, depthStencilView.GetAddressOf());
        if (FAILED(hr))
        {
            throw 8;
        }

        // Setup depth/stencil state, this is the rules for the depth/stencil tests.
        D3D11_DEPTH_STENCIL_DESC depthStencilStateDescription;
        ZeroMemory(&depthStencilStateDescription, sizeof(D3D11_DEPTH_STENCIL_DESC));

        depthStencilStateDescription.DepthEnable = TRUE;
        depthStencilStateDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilStateDescription.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilStateDescription.StencilEnable = FALSE;

        hr = device->CreateDepthStencilState(&depthStencilStateDescription, &depthStencilState);
        if (FAILED(hr))
        {
            throw 9;
        }

        // Setup rasterizer state, this is the rules for the rasterizer
        D3D11_RASTERIZER_DESC rasterizerDescription;
        ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));

        rasterizerDescription.AntialiasedLineEnable = FALSE;
        rasterizerDescription.CullMode = D3D11_CULL_BACK;
        rasterizerDescription.DepthBias = 0;
        rasterizerDescription.DepthBiasClamp = 0.0f;
        rasterizerDescription.DepthClipEnable = TRUE;
        rasterizerDescription.FillMode = D3D11_FILL_SOLID;
        rasterizerDescription.FrontCounterClockwise = FALSE; // TODO, when model loading library imported, change this to TRUE
        rasterizerDescription.MultisampleEnable = FALSE;
        rasterizerDescription.ScissorEnable = FALSE;
        rasterizerDescription.SlopeScaledDepthBias = 0.0f;

        // Create the rasterizer state object.
        hr = device->CreateRasterizerState(&rasterizerDescription, &rasterizerState);
        if (FAILED(hr))
        {
            throw 10;
        }

        // Set viewport information
        viewport.Width = width;
        viewport.Height = height;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        // Initialise constant buffers
        initialiseConstantBuffers();

        // TODO Add shader init
        initialiseShaders();
        // TODO Ready rasteriser and output merger
        return 0;
    }

    void GraphicsDX11::BeginRender()
    {
        const float clearColor[]{ 0, 1, 0, 1 };
        deviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
        deviceContext->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    }

    void GraphicsDX11::UpdateProjectionMatrix(CameraNut* camera)
    {
        DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(camera->GetFov()), GetRenderWindowSize().Width / GetRenderWindowSize().Height, 0.001, 100);

        deviceContext->UpdateSubresource(projectionBuffer.Get(), 0, NULL, &projectionMatrix, 0, 0);
    }

    void GraphicsDX11::SetupCameraForFrame(CameraNut* camera)
    {
        CameraBufferData thisFramesData{
            camera->GetViewMatrix(),
            camera->GetPosition()
        };

        deviceContext->UpdateSubresource(cameraBuffer.Get(), 0, NULL, &thisFramesData, 0, 0);
    }

    void GraphicsDX11::Render(MeshNut* toRender, Mat4 worldMatrix)
    {

        WorldBufferData thisNutsWorldBufferData{
            worldMatrix,
            worldMatrix
        };

        deviceContext->UpdateSubresource(worldBuffer.Get(), 0, NULL, &thisNutsWorldBufferData, 0, 0);

        std::shared_ptr<MeshDX11> meshToRender = std::dynamic_pointer_cast<MeshDX11>(toRender->GetMesh());

        const UINT vertexStride = sizeof(Vertex);
        const UINT vertexOffset = 0;
        deviceContext->IASetVertexBuffers(0, 1, meshToRender->GetVertexBuffer().GetAddressOf(), &vertexStride, &vertexOffset);
        deviceContext->IASetIndexBuffer(meshToRender->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

        // Grab material data
        std::shared_ptr<MaterialDX11> materialOnMesh = std::dynamic_pointer_cast<MaterialDX11>(toRender->GetMaterial());
        if (materialOnMesh.get() != nullptr) {
            deviceContext->UpdateSubresource(materialBuffer.Get(), 0, NULL, materialOnMesh->GetBufferData(), 0, 0);
        }

        deviceContext->DrawIndexed(meshToRender->GetIndexCount(), 0, 0);
    }

    void GraphicsDX11::EndRender()
    {
        swapChain->Present(0, 0);
    }

    void GraphicsDX11::RegisterLightForFrame(LightNut* light)
    {
        lightsData.lights[lightsData.lightCount].lightPosition = light->GetPosition(); // TODO SET TO GLOBAL POSITION
        lightsData.lights[lightsData.lightCount].diffuseColor = light->GetDiffuseColor();
        lightsData.lights[lightsData.lightCount].ambientColor = light->GetAmbientColor();
        lightsData.lights[lightsData.lightCount].intensity = light->GetIntensity();
        lightsData.lights[lightsData.lightCount].ambientIntensity = light->GetAmbientIntensity();

        lightsData.lightCount++;

        deviceContext->UpdateSubresource(lightBuffer.Get(), 0, nullptr, &lightsData, 0, 0);
    }

    void GraphicsDX11::ClearFrameLights()
    {
        lightsData.lightCount = 0;
        deviceContext->UpdateSubresource(lightBuffer.Get(), 0, nullptr, &lightsData, 0, 0);
    }

    Vec2 GraphicsDX11::GetRenderWindowSize()
    {
        return V2(viewport.Width, viewport.Height);
    }

    Vec2 GraphicsDX11::GetWindowLocation()
    {
        RECT rect = { NULL };
        Vec2 location;
        if (GetWindowRect(window, &rect)) {
            location.X = rect.left;
            location.Y = rect.top;
        }

        return location;
    }

    ComPtr<ID3D11Buffer> GraphicsDX11::CreateBuffer(void* data, unsigned int size, D3D11_BIND_FLAG bindFlag)
    {
        ComPtr<ID3D11Buffer> toReturn;

        D3D11_BUFFER_DESC bufferDescription;
        ZeroMemory(&bufferDescription, sizeof(D3D11_BUFFER_DESC));

        bufferDescription.BindFlags = bindFlag;
        bufferDescription.ByteWidth = size;
        bufferDescription.CPUAccessFlags = 0;
        bufferDescription.Usage = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA resourceData;
        ZeroMemory(&resourceData, sizeof(D3D11_SUBRESOURCE_DATA));

        resourceData.pSysMem = data;

        HRESULT hr = device->CreateBuffer(&bufferDescription, &resourceData, toReturn.GetAddressOf());
        /*if (FAILED(hr))
        {
           Fail handling?
        }*/

        return toReturn;
    }

    void GraphicsDX11::initialiseConstantBuffers() {

        // Set up projection buffer
        DirectX::XMMATRIX identity = DirectX::XMMatrixIdentity();
        projectionBuffer = CreateBuffer(&identity, sizeof(DirectX::XMMATRIX), D3D11_BIND_CONSTANT_BUFFER);
        deviceContext->VSSetConstantBuffers(0, 1, projectionBuffer.GetAddressOf());
       
        // Set up camera buffer
        CameraBufferData emptyCameraBuffer;
        ZeroMemory(&emptyCameraBuffer, sizeof(CameraBufferData));
        cameraBuffer = CreateBuffer(&emptyCameraBuffer, sizeof(CameraBufferData), D3D11_BIND_CONSTANT_BUFFER);
        deviceContext->VSSetConstantBuffers(1, 1, cameraBuffer.GetAddressOf());
        deviceContext->PSSetConstantBuffers(0, 1, cameraBuffer.GetAddressOf());

        // Set up world buffer
        WorldBufferData emptyWorldBuffer;
        ZeroMemory(&emptyWorldBuffer, sizeof(WorldBufferData));
        worldBuffer = CreateBuffer(&emptyCameraBuffer, sizeof(WorldBufferData), D3D11_BIND_CONSTANT_BUFFER);
        deviceContext->VSSetConstantBuffers(2, 1, worldBuffer.GetAddressOf());

        // Setup material buffer (The struct exists in MaterialDX11)
        MaterialDX11::MaterialDX11Data emptyMaterialData;
        ZeroMemory(&emptyMaterialData, sizeof(MaterialDX11::MaterialDX11Data));
        materialBuffer = CreateBuffer(&emptyMaterialData, sizeof(MaterialDX11::MaterialDX11Data), D3D11_BIND_CONSTANT_BUFFER);
        deviceContext->PSSetConstantBuffers(1, 1, materialBuffer.GetAddressOf());

        // Light buffer setup later
        LightsBufferData emptyLightData;
        ZeroMemory(&emptyLightData, sizeof(LightsBufferData));
        lightBuffer = CreateBuffer(&emptyLightData, sizeof(LightsBufferData), D3D11_BIND_CONSTANT_BUFFER);
        deviceContext->PSSetConstantBuffers(2, 1, lightBuffer.GetAddressOf());


        return;
    }
    void GraphicsDX11::initialiseShaders()
    {
        // Create shaders from compiled shader code
        device->CreateVertexShader(&DX11BaseVertex_CompiledShader, sizeof(DX11BaseVertex_CompiledShader), NULL, baseVertexShader.GetAddressOf());
        device->CreatePixelShader(&DX11BasePixel_CompiledShader, sizeof(DX11BasePixel_CompiledShader), NULL, basePixelShader.GetAddressOf());

        // Create input layout
        D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, textureCoordinate), D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        // Use device to create input layout 
        device->CreateInputLayout(inputLayout, 3, &DX11BaseVertex_CompiledShader, sizeof(DX11BaseVertex_CompiledShader), baseInputLayout.GetAddressOf());

        // Register input layout with input assembler
        deviceContext->IASetInputLayout(baseInputLayout.Get());

        // Set shaders
        deviceContext->VSSetShader(baseVertexShader.Get(), NULL, NULL);
        deviceContext->PSSetShader(basePixelShader.Get(), NULL, NULL);

        // Set primitive type to triangle list
        deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // Set Output merger
        deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
        deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), depthStencilView.Get());
        
        // Set rasteriser
        deviceContext->RSSetViewports(1, &viewport);
        deviceContext->RSSetState(rasterizerState.Get());

        return;
    }
}
#endif