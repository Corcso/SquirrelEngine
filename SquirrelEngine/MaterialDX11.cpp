#include "PCH.h"
#ifdef DX11
#include "MaterialDX11.h"
#include <fstream>

namespace SQ {
	Material* MaterialDX11::Load(std::string path)
	{
		// Create a new material instance
		Material* newMaterial = new MaterialDX11();

		// Load File into json data then close file
		std::ifstream file(path, std::istream::in);
		if (!file.is_open()) return nullptr;

		nlohmann::json jsonData = nlohmann::json::parse(file);

		file.close();


		if (!jsonData["diffuse"].is_null()) newMaterial->SetDiffuseColor(V3(jsonData["diffuse"][0], jsonData["diffuse"][1], jsonData["diffuse"][2]));
		if (!jsonData["specular"].is_null()) newMaterial->SetSpecularColor(V3(jsonData["specular"][0], jsonData["specular"][1], jsonData["specular"][2]));
		if (!jsonData["specularity"].is_null()) newMaterial->SetSpecularity(jsonData["specularity"]);
		if (!jsonData["smoothness"].is_null()) newMaterial->SetSpecularity(jsonData["smoothness"]);

		return newMaterial;
	}
	void MaterialDX11::SetDiffuseColor(Vec3 diffuseColor)
	{
		//this->diffuseColor = diffuseColor;
		bufferData.diffuseColor = diffuseColor;
	}
	void MaterialDX11::SetSpecularColor(Vec3 specularColor)
	{
		//this->specularColor = specularColor;
		bufferData.specularColor = specularColor;
	}
	void MaterialDX11::SetSpecularity(unsigned int specularity)
	{
		//this->specularity = specularity;
		bufferData.specularity = specularity;
	}
	void MaterialDX11::SetSmoothness(float smoothness)
	{
		//this->smoothness = smoothness;
		bufferData.smoothness = smoothness;
	}
	Vec3 MaterialDX11::GetDiffuseColor()
	{
		return V3(bufferData.diffuseColor.x, bufferData.diffuseColor.z, bufferData.diffuseColor.z);
	}
	Vec3 MaterialDX11::GetSpecularColor()
	{
		return V3(bufferData.specularColor.x, bufferData.specularColor.z, bufferData.specularColor.z);
	}
	unsigned int MaterialDX11::GetSpecularity()
	{
		return bufferData.specularity;
	}
	float MaterialDX11::GetSmoothness()
	{
		return bufferData.smoothness;
	}

	MaterialDX11::MaterialDX11Data* MaterialDX11::GetBufferData()
	{
		return &bufferData;
	}
}

#endif // DX11