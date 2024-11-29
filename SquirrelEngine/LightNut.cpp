#include "PCH.h"
#include "LightNut.h"
namespace SQ {
	Nut* SQ::LightNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Cast deserializeInto to our type, call it toWorkOn
		LightNut* toWorkOn = dynamic_cast<LightNut*>(deserializeInto);
		// If toWorkOn is nullptr, make a new nut of our type. 
		if (toWorkOn == nullptr) toWorkOn = new LightNut();
		// Call parent deserialise, passing in our toWorkOn.
		WorldNut::Deserialize(toWorkOn, serializedData);

		// Perform deserialization on our data. 
		if (!serializedData["diffuseColor"].is_null());

		// Return toWorkOn
		return toWorkOn;
	}
	void LightNut::SetDiffuseColor(Vec3 diffuseColor)
	{
		this->diffuseColor = diffuseColor;
	}
	void LightNut::SetAmbientColor(Vec3 ambientColor)
	{
		this->ambientColor = ambientColor;
	}
	void LightNut::SetIntensity(float intensity)
	{
		this->intensity = intensity;
	}
	void LightNut::SetAmbientIntensity(float ambientIntensity)
	{
		this->ambientIntensity = ambientIntensity;
	}
	Vec3 LightNut::GetDiffuseColor()
	{
		return diffuseColor;
	}
	Vec3 LightNut::GetAmbientColor()
	{
		return ambientColor;
	}
	float LightNut::GetIntensity()
	{
		return intensity;
	}
	float LightNut::GetAmbientIntensity()
	{
		return ambientIntensity;
	}
}