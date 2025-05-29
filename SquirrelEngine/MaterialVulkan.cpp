#include "PCH.h"

#ifdef VULKAN
#include "MaterialVulkan.h"
#include <fstream>

namespace SQ {
	Material* MaterialVulkan::Load(std::string path)
	{
		// Create a new material instance
		Material* newMaterial = new MaterialVulkan();

		// Load File into json data then close file
		std::ifstream file(path, std::istream::in);
		if (!file.is_open()) return nullptr;

		nlohmann::json jsonData = nlohmann::json::parse(file);

		file.close();


		if (!jsonData["diffuse"].is_null()) newMaterial->SetDiffuseColor(V3(jsonData["diffuse"][0], jsonData["diffuse"][1], jsonData["diffuse"][2]));
		if (!jsonData["specular"].is_null()) newMaterial->SetSpecularColor(V3(jsonData["specular"][0], jsonData["specular"][1], jsonData["specular"][2]));
		if (!jsonData["specularity"].is_null()) newMaterial->SetSpecularity(jsonData["specularity"]);
		if (!jsonData["smoothness"].is_null()) newMaterial->SetSmoothness(jsonData["smoothness"]);

		return newMaterial;
	}
	void MaterialVulkan::SetDiffuseColor(Vec3 diffuseColor)
	{
		//this->diffuseColor = diffuseColor;
		bufferData.diffuseColor = diffuseColor;
	}
	void MaterialVulkan::SetSpecularColor(Vec3 specularColor)
	{
		//this->specularColor = specularColor;
		bufferData.specularColor = specularColor;
	}
	void MaterialVulkan::SetSpecularity(unsigned int specularity)
	{
		//this->specularity = specularity;
		bufferData.specularity = specularity;
	}
	void MaterialVulkan::SetSmoothness(float smoothness)
	{
		//this->smoothness = smoothness;
		bufferData.smoothness = smoothness;
	}
	Vec3 MaterialVulkan::GetDiffuseColor()
	{
		return bufferData.diffuseColor;
	}
	Vec3 MaterialVulkan::GetSpecularColor()
	{
		return bufferData.specularColor;
	}
	unsigned int MaterialVulkan::GetSpecularity()
	{
		return bufferData.specularity;
	}
	float MaterialVulkan::GetSmoothness()
	{
		return bufferData.smoothness;
	}

	MaterialVulkan::MaterialVulkanData* MaterialVulkan::GetBufferData()
	{
		return &bufferData;
	}
}

#endif // VULKAN