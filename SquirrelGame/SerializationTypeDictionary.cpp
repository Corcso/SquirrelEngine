// Squirrel Engine Includes
#include "SquirrelEngine.h"
#include "SerializationTypeDictionary.h"

// ====================================
// PLACE YOUR INCLUDES HERE
#include "FPVCamera.h"
// ====================================

// Dictionary setup
std::unordered_map<std::string, std::function<SQ::Nut*(SQ::Nut*, nlohmann::json)>> SQ::SerializationTypeDictionary({
	// Squirrel Engine Nuts, Editing is not recomended.
	{"Nut", SQ::Nut::Deserialize},
	{"WorldNut", SQ::WorldNut::Deserialize},
	{"MeshNut", SQ::MeshNut::Deserialize},
	{"CameraNut", SQ::CameraNut::Deserialize},

// ====================================
// PLACE YOUR NUT DESERIALIZE FUNCTIONS HERE!
// {"Name Of Type In .nut file", MYCLASS::Deserialize}
	{"FPVCamera", FPVCamera::Deserialize}

	
	
// ====================================

});