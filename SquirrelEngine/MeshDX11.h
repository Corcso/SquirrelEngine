#pragma once
#include "Mesh.h"

#include "SquirrelEnginePI.h"

namespace SQ {
    class MeshDX11 :
        public Mesh
    {
    public:
        /// <summary>
        /// Loads buffers with data. 
        /// </summary>
        void LoadBuffers();

        /// <summary>
        /// Get vertex buffer for DX11 use
        /// </summary>
        /// <returns>Vertex buffer</returns>
        ComPtr<ID3D11Buffer> GetVertexBuffer();

        /// <summary>
        /// Get index buffer for DX11 use
        /// </summary>
        /// <returns>Index buffer</returns>
        ComPtr<ID3D11Buffer> GetIndexBuffer();

        virtual ~MeshDX11() {};
    private:
        // Define the index and vertex buffer
        ComPtr<ID3D11Buffer> vertexBuffer;
        ComPtr<ID3D11Buffer> indexBuffer;
    };
}

