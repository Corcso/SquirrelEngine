#include "PCH.h"
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11

#include "HandmadeMath.h"

// For testing
#include <iostream>

int main() {
	std::unique_ptr<SQ::Graphics> graphicsService;
#ifdef DX11
	graphicsService.reset(new SQ::GraphicsDX11());
#endif // DX11

	SQ::Vec4 myVector = SQ::V4(1, 2, 0, 1);
	SQ::Mat4 matrix{ 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, -2, 1};

	SQ::Vec4 myVectorOut = matrix * myVector;
	
	DirectX::XMFLOAT4 myV = myVector;
	std::cout << myV.x << myV.y << myV.z;
	
	return graphicsService->init(500, 200);
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS