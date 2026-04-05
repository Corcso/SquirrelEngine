#include "PCH.h"
#include "Material.h"
#ifdef DX11
#include "MaterialDX11.h"
#endif // DX11
#ifdef VULKAN
#include "MaterialVulkan.h"
#endif // VULKAN

#include <fstream>

namespace SQ {
    StandardMaterial* StandardMaterial::Load(std::string path)
    {

        

		// Create a new material instance
#ifdef DX11
		StandardMaterial* newMaterial = new MaterialDX11();
#endif // DX11
#ifdef VULKAN
		StandardMaterial* newMaterial = new MaterialVulkan();
#endif // VULKAN

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
    void StandardMaterial::ImGuiRenderMyInspector()
    {
        Resource::ImGuiRenderMyInspector();
        if (ImGui::TreeNodeEx("StandardMaterial", ImGuiTreeNodeFlags_DefaultOpen)) {

			ImGui::ColorEdit3("Diffuse Color", reinterpret_cast<float*>(&diffuseColor));
			ImGui::ColorEdit3("Specular Color", reinterpret_cast<float*>(&specularColor));
			ImGui::SliderInt("Speculariy", reinterpret_cast<int*>(&specularity), 1, 256);
			ImGui::SliderFloat("Smoothness", &smoothness, 0, 1);

			ImGui::TreePop();
        }
    }

	void StandardMaterial::SetDiffuseColor(Vec3 diffuseColor)
	{
		this->diffuseColor = diffuseColor;
	}
	void StandardMaterial::SetSpecularColor(Vec3 specularColor)
	{
		this->specularColor = specularColor;
	}
	void StandardMaterial::SetSpecularity(unsigned int specularity)
	{
		this->specularity = specularity;
	}
	void StandardMaterial::SetSmoothness(float smoothness)
	{
		this->smoothness = smoothness;
	}
	Vec3 StandardMaterial::GetDiffuseColor()
	{
		return diffuseColor;
	}
	Vec3 StandardMaterial::GetSpecularColor()
	{
		return specularColor;
	}
	unsigned int StandardMaterial::GetSpecularity()
	{
		return specularity;
	}
	float StandardMaterial::GetSmoothness()
	{
		return smoothness;
	}
}
