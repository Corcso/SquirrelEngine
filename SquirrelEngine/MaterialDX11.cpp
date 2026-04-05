#include "PCH.h"
#ifdef DX11
#include "MaterialDX11.h"
#include <fstream>

namespace SQ {
	StandardMaterial* MaterialDX11::Load(std::string path)
	{
		// Create a new material instance
		StandardMaterial* newMaterial = new MaterialDX11();

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

	MaterialDX11::MaterialDX11Data* MaterialDX11::GetBufferData()
	{
		bufferData.diffuseColor = diffuseColor;
		bufferData.specularColor = specularColor;
		bufferData.smoothness = smoothness;
		bufferData.specularity = specularity;

		return &bufferData;
	}
	void MaterialDX11::ImGuiRenderMyInspector()
	{
		StandardMaterial::ImGuiRenderMyInspector();
		if (ImGui::TreeNodeEx("MaterialDX11", ImGuiTreeNodeFlags_DefaultOpen)) {

			ImGui::ColorEdit3("Diffuse Color", reinterpret_cast<float*>(&bufferData.diffuseColor));
			ImGui::ColorEdit3("Specular Color", reinterpret_cast<float*>(&bufferData.specularColor));
			ImGui::SliderInt("Speculariy", reinterpret_cast<int*>(&bufferData.specularity), 1, 256);
			ImGui::SliderFloat("Smoothness", &bufferData.smoothness, 0, 1);

			ImGui::TreePop();
		}
	}
}

#endif // DX11