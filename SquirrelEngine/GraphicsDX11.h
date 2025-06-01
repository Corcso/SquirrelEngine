#pragma once

#ifdef DX11

#include "SquirrelEnginePI.h"

#include "Graphics.h"
namespace SQ {
    class GraphicsDX11 :
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

        virtual void RegisterWindowSizeChange(Vec2 newSize) final {};

        /// <summary>
        /// Creates a buffer on the GPU
        /// </summary>
        /// <param name="data">Data to prefill with</param>
        /// <param name="size">Size of the buffer</param>
        /// <param name="bindFlag">Bind flags to use</param>
        /// <returns></returns>
        ComPtr<ID3D11Buffer> CreateBuffer(void* data, unsigned int size, D3D11_BIND_FLAG bindFlag);
    private:
        // Windows Window Handle
        HWND window;

        // Window class name must be unique
        const LPCWSTR WINDOW_CLASS_NAME = L"SquirrelEngineGame";

        // Clear colour
        Vec4 clearColor;

        // Device and device context used for GPU communication
        ComPtr<ID3D11Device> device;
        ComPtr<ID3D11DeviceContext> deviceContext;

        // Swap chain used for back and front buffers
        ComPtr<IDXGISwapChain> swapChain;

        // Render Target View for Back Buffer
        ComPtr<ID3D11RenderTargetView> renderTargetView;

        // Viewport
        D3D11_VIEWPORT viewport;

        // Depth/stencil view for use as a depth buffer.
        ComPtr<ID3D11DepthStencilView> depthStencilView;

        // Depth/stencil buffer
        ComPtr<ID3D11Texture2D> depthStencilBuffer;

        // Depth stencil state for depth and stencil buffer rules
        ComPtr<ID3D11DepthStencilState> depthStencilState;

        // Rasterizer state for rasterizer rules
        ComPtr<ID3D11RasterizerState> rasterizerState;

        // Constant Buffers
        ComPtr<ID3D11Buffer> projectionBuffer; // For storing projection matrix
        ComPtr<ID3D11Buffer> cameraBuffer; // For storing camera info & view matrix
        ComPtr<ID3D11Buffer> worldBuffer; // For storing world matrix & positional information

        ComPtr<ID3D11Buffer> materialBuffer; // For storing material information
        ComPtr<ID3D11Buffer> lightBuffer; // For storing lighting information

        // Stores the layout of the data for the camera buffer
        struct CameraBufferData {
            DirectX::XMMATRIX viewMatrix;
            DirectX::XMFLOAT3A cameraPosition;
        };

        // Stores the layout of the data for the world buffer
        struct WorldBufferData {
            DirectX::XMMATRIX worldMatrix;
            DirectX::XMMATRIX normalWorldMatrix;
        };


        /// <summary>
        /// Initialises the constant buffers which are used in the base shaders. 
        /// </summary>
        void initialiseConstantBuffers();

        ComPtr<ID3D11VertexShader> baseVertexShader;
        ComPtr<ID3D11PixelShader> basePixelShader;
        ComPtr<ID3D11InputLayout> baseInputLayout;

        /// <summary>
        /// Initialises base shaders.
        /// </summary>
        void initialiseShaders();

        // Lighting
        // p_X is padding for GPU
        struct LightBufferData {
            DirectX::XMFLOAT3 lightPosition; float p_0;

            DirectX::XMFLOAT3 lightDirection; float p_1;

            DirectX::XMFLOAT3 diffuseColor; float p_2;

            DirectX::XMFLOAT3 ambientColor;
            float intensity;

            float ambientIntensity;
            unsigned int lightType;
            float innerCutoffAngle;
            float outerCutoffAngle;

            // No need for constant attenuation, this can be edited by changing intensity. 
            float linearAttenuation;
            float quadraticAttenuation;
            float p_3;
            float p_4;
        };

        struct LightsBufferData {
            LightBufferData lights[8];
            unsigned int lightCount;

            float p_0;
            float p_1;
            float p_2;
        };

        LightsBufferData lightsData;
    };
}
#endif