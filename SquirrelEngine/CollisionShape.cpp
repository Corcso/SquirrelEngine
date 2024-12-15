#include "PCH.h"
#include "CollisionShape.h"
#include <fstream>
#include "PhysicsNut.h"
#include "Services.h"

namespace SQ {
    CollisionShape* SQ::CollisionShape::Load(std::string path)
    {
		// Create a new material instance
		CollisionShape* newShape = new CollisionShape();

		// Load File into json data then close file
		std::ifstream file(path, std::istream::in);
		if (!file.is_open()) return nullptr;

		nlohmann::json jsonData = nlohmann::json::parse(file);

		file.close();

		if (!jsonData["type"].is_null()) { 
			if (jsonData["type"] == "box") newShape->type = Type::BOX;
			else if (jsonData["type"] == "sphere") newShape->type = Type::SPHERE;
			else if (jsonData["type"] == "capsule") newShape->type = Type::CAPSULE;
			else newShape->type = Type::SPHERE;
		}

		if (!jsonData["boxHalfDimentions"].is_null()) newShape->boxHalfDimentions = V3(jsonData["boxHalfDimentions"][0], jsonData["boxHalfDimentions"][1], jsonData["boxHalfDimentions"][2]);
		if (!jsonData["sphereRadius"].is_null()) newShape->sphereRadius = jsonData["sphereRadius"];
		if (!jsonData["capsuleRadius"].is_null()) newShape->capsuleRadius = jsonData["capsuleRadius"];
		if (!jsonData["capsuleHalfHeight"].is_null()) newShape->capsuleHalfHeight = jsonData["capsuleHalfHeight"];

		newShape->Updated();

		return newShape;
    }

	void CollisionShape::SetType(Type type)
	{
		this->type = type;
		Updated();
	}

	void CollisionShape::SetBoxHalfDimentions(Vec3 halfDimentions)
	{
		boxHalfDimentions = halfDimentions;
		if (type == Type::BOX) Updated();
	}

	void CollisionShape::SetCapsuleRadius(float radius)
	{
		capsuleRadius = radius;
		if (type == Type::CAPSULE) Updated();
	}

	void CollisionShape::SetCapsuleHalfHeight(float halfHeight)
	{
		capsuleHalfHeight = halfHeight;
		if (type == Type::CAPSULE) Updated();
	}

	void CollisionShape::SetSphereRadius(float radius)
	{
		sphereRadius = radius;
		if (type == Type::SPHERE) Updated();
	}

	CollisionShape::Type CollisionShape::GetType()
	{
		return type;
	}
	Vec3 CollisionShape::GetBoxHalfDimentions()
	{
		return boxHalfDimentions;
	}
	float CollisionShape::GetCapsuleRadius()
	{
		return capsuleRadius;
	}
	float CollisionShape::GetCapsuleHalfHeight()
	{
		return capsuleHalfHeight;
	}
	float CollisionShape::GetSphereRadius()
	{
		return sphereRadius;
	}
	void CollisionShape::Updated()
	{
		for (std::set<PhysicsNut*>::iterator it = activeOnTheseNuts.begin(); it != activeOnTheseNuts.end(); ++it) {
			Services::GetPhysics()->BodyShapeUpdated(*it);
		}
	}
	void CollisionShape::RegisterNut(PhysicsNut* nut)
	{
		activeOnTheseNuts.insert(nut);
	}
	void CollisionShape::RemoveNut(PhysicsNut* nut)
	{
		activeOnTheseNuts.erase(nut);
	}
}