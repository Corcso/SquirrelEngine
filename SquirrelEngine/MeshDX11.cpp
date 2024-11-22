#include "PCH.h"
#ifdef DX11
#include "MeshDX11.h"
#include "Services.h"

namespace SQ {
	void MeshDX11::LoadBuffers()
	{
		// Add buffer creation and loading here, 
		// Talk to GraphicsDX11 Service need service locator pattern decided. 

		// Get graphics service which we can assume is DX11 version as we are DX11 mesh
		GraphicsDX11* graphicsService = dynamic_cast<GraphicsDX11*>(Services::GetGraphics());

		// Create the vertex buffer
		vertexBuffer = graphicsService->CreateBuffer(vertices.data(), sizeof(Vertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
		// Create the index buffer
		indexBuffer = graphicsService->CreateBuffer(indicies.data(), sizeof(unsigned int) * indicies.size(), D3D11_BIND_INDEX_BUFFER);
	}

	ComPtr<ID3D11Buffer> MeshDX11::GetVertexBuffer()
	{
		return vertexBuffer;
	}

	ComPtr<ID3D11Buffer> MeshDX11::GetIndexBuffer()
	{
		return indexBuffer;
	}
}

#endif // DX11