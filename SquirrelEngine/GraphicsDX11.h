#pragma once

#ifdef DX11

// DirectX includes
#include <d3d11.h>
//#include <d3dcompiler.h>
#include <DirectXMath.h>
//#include <DirectXColors.h>
#include <wrl.h>

// Link library dependencies
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "dxgi.lib") I've commented these out only going to comment them in when I think I need them, same above
//#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "winmm.lib")

// Define ComPtr as Microsoft::WRL::ComPtr for ease of use.
template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#include "Graphics.h"
namespace SQ {
    class GraphicsDX11 :
        public Graphics
    {
    public:
        virtual int Init(int width, int height) final;


        virtual void BeginRender() final;
        virtual void UpdateProjectionMatrix(CameraNut* camera) final;

        virtual void SetupCameraForFrame(CameraNut* camera) final;
        virtual void Render(MeshNut* toRender, Mat4 worldMatrix) final;
        virtual void EndRender() final;

        virtual Vec2 GetRenderWindowSize() final;
        virtual Vec2 GetWindowLocation() final;

        ComPtr<ID3D11Buffer> CreateBuffer(void* data, unsigned int size, D3D11_BIND_FLAG bindFlag);
    private:
        // Windows Window Handle
        HWND window;

        // Window class name must be unique
        const LPCWSTR WINDOW_CLASS_NAME = L"SquirrelEngineGame";

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
    };
}
#endif