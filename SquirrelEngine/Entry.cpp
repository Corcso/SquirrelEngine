#include "PCH.h"
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS

#include "Services.h"

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11

#include "Math.h"

// For testing
#include <iostream>
#include "Mesh.h"

int main() {
#ifdef DX11
	SQ::Services::RegisterGraphics(new SQ::GraphicsDX11());
#endif // DX11

	/*SQ::Vec4 myVector = SQ::V4(1, 2, 0, 1);
	SQ::Mat4 matrix{ 
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, -2, 1};

	SQ::Vec4 myVectorOut = matrix * myVector;
	
	DirectX::XMFLOAT4 myV = myVector;
	std::cout << myV.x << myV.y << myV.z;*/
	SQ::Services::GetGraphics()->Init(500, 200);
	SQ::Mesh::Load("./Resources/TestQuad.obj");

	return 0;
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS