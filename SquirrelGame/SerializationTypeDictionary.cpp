// Squirrel Engine Includes
#include "SquirrelEngine.h"
#include "SerializationTypeDictionary.h"

// Helpers
#define RESOURCE_SAVE_NOT_IMPLEMENTED [](const Resource& a, std::string b) {std::cout << "Resource Save Not Implemented.\n";}
#define RESOURCE_LOAD_HELPER(ClassName) [](std::string path) {return GetResourceManager()->Retrieve<ClassName>(path); }
#define RESOURCE_RELOAD_HELPER(ClassName) [](std::string path) {return GetResourceManager()->Reload<ClassName>(path); }
#define RESOURCE_CREATE_HELPER(ClassName) []() {return new ClassName(); }
#define RESOURCE_SAVE_HELPER(ClassName) [](const Resource& resource, std::string path) {return ClassName::Save(dynamic_cast<const ClassName&>(resource), path); }

// ====================================
// PLACE YOUR NUT INCLUDES HERE
#include "FPVCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "AirlockSceneLoading.h"
// ====================================

// Dictionary setup
std::unordered_map<std::string, std::function<SQ::UniquePoolPtr<Nut>(Nut*, nlohmann::json)>> SQ::SerializationTypeDictionary({
	// Squirrel Engine Nuts, Editing is not recomended.
	{"Nut", SQ::Nut::Deserialize},
	{"WorldNut", SQ::WorldNut::Deserialize},
	{"MeshNut", SQ::MeshNut::Deserialize},
	{"CameraNut", SQ::CameraNut::Deserialize},
	{"LightNut", SQ::LightNut::Deserialize},
	{"PhysicsNut", SQ::PhysicsNut::Deserialize},

// ====================================
// PLACE YOUR NUT DESERIALIZE FUNCTIONS HERE!
// {"Name Of Type In .nut file", MYCLASS::Deserialize}
	{"FPVCamera", FPVCamera::Deserialize},
	{"Player", Player::Deserialize},
	{"Enemy", Enemy::Deserialize},
	{"Bullet", Bullet::Deserialize},
	{"AirlockSceneLoading", AirlockSceneLoading::Deserialize}
// ====================================

});

// ====================================
// PLACE YOUR RESOURCE INCLUDES HERE

// ====================================

std::unordered_map<std::string, SQ::ResourceInteractionInformation> SQ::ResourceTypeDictionary({
	{".nut", {"Shelled Nut", RESOURCE_LOAD_HELPER(ShelledNut), RESOURCE_RELOAD_HELPER(ShelledNut), RESOURCE_CREATE_HELPER(ShelledNut), RESOURCE_SAVE_NOT_IMPLEMENTED}},
	{".shape", {"Collision Shape", RESOURCE_LOAD_HELPER(CollisionShape), RESOURCE_RELOAD_HELPER(CollisionShape), RESOURCE_CREATE_HELPER(CollisionShape), RESOURCE_SAVE_NOT_IMPLEMENTED}},
	{".mat", {"Material", RESOURCE_LOAD_HELPER(StandardMaterial), RESOURCE_RELOAD_HELPER(StandardMaterial), RESOURCE_CREATE_HELPER(StandardMaterial), RESOURCE_SAVE_HELPER(StandardMaterial)}},

	// ====================================
	// External Types 
	{".obj", {"Mesh (OBJ Wavefront)", RESOURCE_LOAD_HELPER(Mesh), RESOURCE_RELOAD_HELPER(Mesh), RESOURCE_CREATE_HELPER(Mesh), RESOURCE_SAVE_NOT_IMPLEMENTED}},
	// ====================================

	// ====================================
	// PLACE YOUR RESOURCE FUNCTIONS HERE!
	// {"file extension with .", {MYRESOURCE::Load, [](){return new MYRESOURCE();}}}
	
	// ====================================
});