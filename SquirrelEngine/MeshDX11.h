#pragma once
#include "Mesh.h"

#include "SquirrelEnginePI.h"

namespace SQ {
    class MeshDX11 :
        public Mesh
    {
    public:
        
        void LoadBuffers();

        ComPtr<ID3D11Buffer> GetVertexBuffer();
        ComPtr<ID3D11Buffer> GetIndexBuffer();

        virtual ~MeshDX11() {};
    private:
        // Define the index and vertex buffer
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
    };
}

