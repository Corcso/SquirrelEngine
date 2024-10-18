#include "PCH.h"
#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11


int main() {
	std::unique_ptr<Graphics> graphicsService;
#ifdef DX11
	graphicsService.reset(new GraphicsDX11());
#endif // DX11
	return graphicsService->init(500, 200);
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS