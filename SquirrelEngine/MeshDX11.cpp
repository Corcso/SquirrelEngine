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

		graphicsService->CreateBuffer(vertices.data(), sizeof(Vertex) * vertices.size());
	}
}

#endif // DX11