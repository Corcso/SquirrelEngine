#include "PCH.h"
#ifdef DX11
#include "MaterialDX11.h"

namespace SQ {
	void MaterialDX11::SetDiffuseColor(Vec3 diffuseColor)
	{
		this->diffuseColor = diffuseColor;
		bufferData.diffuseColor = diffuseColor;
	}
	void MaterialDX11::SetSpecularColor(Vec3 specularColor)
	{
		this->specularColor = specularColor;
		bufferData.specularColor = specularColor;
	}
	void MaterialDX11::SetSpecularity(unsigned int specularity)
	{
		this->specularity = specularity;
		bufferData.specularity = specularity;
	}
	void MaterialDX11::SetSmoothness(float smoothness)
	{
		this->smoothness = smoothness;
		bufferData.smoothness = smoothness;
	}
	Vec3 MaterialDX11::GetDiffuseColor()
	{
		return diffuseColor;
	}
	Vec3 MaterialDX11::GetSpecularColor()
	{
		return specularColor;
	}
	unsigned int MaterialDX11::GetSpecularity()
	{
		return specularity;
	}
	float MaterialDX11::GetSmoothness()
	{
		return smoothness;
	}

	MaterialDX11::MaterialDX11Data MaterialDX11::GetBufferData()
	{
		return bufferData;
	}
}

#endif // DX11