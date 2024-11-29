#include "PCH.h"
#include "SquirrelEngine.h"


#ifdef WINDOWS
#include "InputWindows.h"
#endif // WINDOWS

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11

#include <fstream>

/// <summary>
/// Entry point.
/// </summary>
/// <returns></returns>
int main() {
	// Load the options file
	std::ifstream file("options.json", std::istream::in);
	if (!file.is_open()) return 0;
	nlohmann::json options = nlohmann::json::parse(file);
	file.close();

	// Check all required options parameters are set
	if (options["Window Name"].is_null()) return 0;
	if (options["Window Size"].is_null()) return 0;
	if (!options["Window Size"].is_array()) return 0;
	if (options["On Load Nut"].is_null()) return 0;

	// REGISTER SERVICES
#ifdef WINDOWS
	SQ::Services::RegisterInput(new SQ::InputWindows());
#endif // WINDOWS
#ifdef DX11
	SQ::Services::RegisterGraphics(new SQ::GraphicsDX11());
#endif // DX11

	SQ::Services::RegisterResourceManager(new SQ::ResourceManager());
	
	SQ::Services::RegisterTree(new SQ::Tree());

	// Init Graphics
	SQ::Services::GetGraphics()->Init(options["Window Name"], options["Window Size"][0], options["Window Size"][1]);

	// Init Tree 	
	SQ::Services::GetTree()->Init(options["On Load Nut"]);

	// Unlock mouse TEMP
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