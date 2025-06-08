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

		// Deserialize data
		if (!jsonData["type"].is_null()) { 
			if (jsonData["type"] == "box") newShape->type = Type::BOX;
			else if (jsonData["type"] == "sphere") newShape->type = Type::SPHERE;
			else if (jsonData["type"] == "capsule") newShape->type = Type::CAPSULE;
			else newShape->type = Type::SPHERE;
		}

		if (!jsonData["boxHalfDimensions"].is_null()) newShape->boxHalfDimensions = V3(jsonData["boxHalfDimensions"][0], jsonData["boxHalfDimensions"][1], jsonData["boxHalfDimensions"][2]);
		if (!jsonData["sphereRadius"].is_null()) newShape->sphereRadius = jsonData["sphereRadius"];
		if (!jsonData["capsuleRadius"].is_null()) newShape->capsuleRadius = jsonData["capsuleRadius"];
		if (!jsonData["capsuleHalfHeight"].is_null()) newShape->capsuleHalfHeight = jsonData["capsuleHalfHeight"];

		// Update physics system
		newShape->Updated();

		return newShape;
    }

	void CollisionShape::SetType(Type type)
	{
		this->type = type;
		Updated();
	}

	void CollisionShape::SetBoxHalfDimensions(Vec3 halfDimentions)
	{
		boxHalfDimensions = halfDimentions;
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
	Vec3 CollisionShape::GetBoxHalfDimensions()
	{
		return boxHalfDimensions;
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
	void CollisionShape::ImGuiRenderMyInspector()
	{
		Resource::ImGuiRenderMyInspector();
		if (ImGui::TreeNodeEx("CollisionShape", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::BeginDisabled();
			
			const char* items[] = { "BOX", "CAPSULE", "SPHERE" };
			ImGui::Combo("combo", reinterpret_cast<int*>(&type), items, IM_ARRAYSIZE(items));

			if(type == Type::BOX) ImGui::DragFloat3("Box Half Dimentions", reinterpret_cast<float*>(&boxHalfDimensions));
			if (type == Type::CAPSULE) {
				ImGui::DragFloat("Capsule Half Height", &capsuleHalfHeight);
				ImGui::DragFloat("Capsule Radius", &capsuleRadius);
			}
			if (type == Type::SPHERE) { ImGui::DragFloat("Sphere Radius", &sphereRadius); }
			
			ImGui::EndDisabled();
			ImGui::TreePop();
		}
	}
}