#include "PCH.h"
#include "SquirrelEngine.h"


#ifdef WINDOWS
#include "InputWindows.h"
#endif // WINDOWS

#ifdef DX11
#include "GraphicsDX11.h"
#endif // DX11

#ifdef JOLT
#include "PhysicsJolt.h"
#endif // JOLT

#include <fstream>

/// <summary>
/// Entry point.
/// </summary>
/// <returns></returns>
int main() {
	// Entire program wrapped in a try catch 
	try {
		// Load the options file
		std::ifstream file("options.json", std::istream::in);
		if (!file.is_open()) throw 11;
		nlohmann::json options = nlohmann::json::parse(file);
		file.close();

		// Check all required options parameters are set
		if (options["Window Name"].is_null()) return 12;
		if (options["Window Size"].is_null()) return 13;
		if (!options["Window Size"].is_array()) return 14;
		if (options["On Load Nut"].is_null()) return 15;
		if (options["Background Color"].is_null()) return 20;
		if (!options["Background Color"].is_array()) return 21;


		// REGISTER SERVICES (Order of registration does not matter)
#ifdef WINDOWS
		SQ::Services::RegisterInput(new SQ::InputWindows());
#endif // WINDOWS
#ifdef DX11
		SQ::Services::RegisterGraphics(new SQ::GraphicsDX11());
#endif // DX11
#ifdef JOLT
		SQ::Services::RegisterPhysics(new SQ::PhysicsJolt());
#endif // JOLT

		SQ::Services::RegisterResourceManager(new SQ::ResourceManager());

		SQ::Services::RegisterTree(new SQ::Tree());
		SQ::Services::RegisterTime(new SQ::Time());
		SQ::Services::RegisterPoolAllocationService(new SQ::PoolAllocationService());

		// INITIALISE SERVICES order does matter

		// Init Graphics
		SQ::Services::GetGraphics()->Init(options["Window Name"], options["Window Size"][0], options["Window Size"][1], SQ::V4(options["Background Color"][0], options["Background Color"][1], options["Background Color"][2], 1));
		// Init Physics
		SQ::Services::GetPhysics()->Init();
		// Init Pool Allocation
		SQ::Services::GetPoolAllocationService()->Init();

		// Init Tree 	
		SQ::Services::GetTree()->Init(options["On Load Nut"]);
		
		// Init Time
		SQ::Services::GetTime()->Init();

		// There is no init for input

		// Default mouse to be unlocked
		SQ::Services::GetInput()->UnlockMouse();

		// Begin game loop
		SQ::Services::GetTree()->RunLoop();

		return 0;
	}
	catch (int error) {
		return error;
	}
}

#ifdef WINDOWS
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	return main();
}
#endif // WINDOWS