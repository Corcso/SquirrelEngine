#include "PCH.h"

#ifdef VULKAN
#include "MaterialVulkan.h"
#include <fstream>

namespace SQ {
	StandardMaterial* MaterialVulkan::Load(std::string path)
	{
		// Create a new material instance
		StandardMaterial* newMaterial = new MaterialVulkan();

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

	MaterialVulkan::MaterialVulkanData* MaterialVulkan::GetBufferData()
	{
		bufferData.diffuseColor = diffuseColor;
		bufferData.specularColor = specularColor;
		bufferData.smoothness = smoothness;
		bufferData.specularity = specularity;

		return &bufferData;
	}
}

#endif // VULKAN