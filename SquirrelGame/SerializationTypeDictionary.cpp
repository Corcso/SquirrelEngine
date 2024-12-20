// Squirrel Engine Includes
#include "SquirrelEngine.h"
#include "SerializationTypeDictionary.h"

// ====================================
// PLACE YOUR INCLUDES HERE
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