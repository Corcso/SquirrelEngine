#include "PCH.h"
#include "SquirrelEngine.h"

#ifdef WINDOWS
#include <Windows.h>
#endif // WINDOWS

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11
#ifdef WINDOWS
#include "InputWindows.h"
#endif // WINDOWS


// For testing
#include <iostream>


int main() {

#ifdef WINDOWS
	SQ::Services::RegisterInput(new SQ::InputWindows());
#endif // WINDOWS
#ifdef DX11
	SQ::Services::RegisterGraphics(new SQ::GraphicsDX11());
#endif // DX11
	SQ::Services::GetGraphics()->Init(1600, 900);

	// Setup resource manager
	SQ::Services::RegisterResourceManager(new SQ::ResourceManager());
	SQ::Services::GetResourceManager();

	// Setup tree 
	SQ::Services::RegisterTree(new SQ::Tree());
	SQ::Services::GetTree()->Init();

	// Unlock mouse temp
	SQ::Services::GetInput()->UnlockMouse();

	// Begin game loop
	SQ::Services::GetTree()->RunLoop();

	return 0;
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS