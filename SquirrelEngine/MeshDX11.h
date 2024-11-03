#pragma once
#include "Mesh.h"

// This is tempoary before I get a global DX11 API header
#include "GraphicsDX11.h"

namespace SQ {
    class MeshDX11 :
        public Mesh
    {
    public:
        
        void LoadBuffers();

        virtual ~MeshDX11() {};
    private:
        // Define the index and vertex buffer
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
    };
}

