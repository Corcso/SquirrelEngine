#include "PCH.h"
#include "LightNut.h"
#include "Services.h"
namespace SQ {
	LightNut::LightNut()
	{
		diffuseColor = V3(1, 1, 1);
		ambientColor = V3(1, 1, 1);
		intensity = 1;
		ambientIntensity = 0.1;
		SetLightType(LightType::POINT);
		spotInnerAngle = 0;
		spotOuterAngle = 0;
		linearAttenuation = 0;
		quadraticAttenuation = 0;
	}
	UniquePoolPtr<Nut> SQ::LightNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
	{
		// Cast deserializeInto to our type, call it toWorkOn
		LightNut* toWorkOn = dynamic_cast<LightNut*>(deserializeInto);
		// If toWorkOn is nullptr, make a new nut of our type. 
		// We need to follow strict ownership with the pool ptr
		UniquePoolPtr<Nut> owner;
		if (toWorkOn == nullptr) {
			// Get the instance
			UniquePoolPtr<LightNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<LightNut>();
			// Set to work on to the instance
			toWorkOn = instance.get();
			// Transfer ownership into owner and static cast to nut base class
			owner = instance.StaticUniquePoolPtrCast<Nut>();
		}
		// Call parent deserialise, passing in our toWorkOn.
		WorldNut::Deserialize(toWorkOn, serializedData);

		// Perform deserialization on our data. 
		if (!serializedData["diffuseColor"].is_null()) toWorkOn->SetDiffuseColor(V3(serializedData["diffuseColor"][0], serializedData["diffuseColor"][1], serializedData["diffuseColor"][2]));
		if (!serializedData["ambientColor"].is_null()) toWorkOn->SetAmbientColor(V3(serializedData["ambientColor"][0], serializedData["ambientColor"][1], serializedData["ambientColor"][2]));
		if (!serializedData["intensity"].is_null()) toWorkOn->SetIntensity(serializedData["intensity"]);
		if (!serializedData["ambientIntensity"].is_null()) toWorkOn->SetAmbientIntensity(serializedData["ambientIntensity"]);
		
		if (!serializedData["lightType"].is_null()) {
			if(serializedData["lightType"] == "directional") toWorkOn->SetLightType(LightType::DIRECTIONAL);
			else if(serializedData["lightType"] == "point") toWorkOn->SetLightType(LightType::POINT);
			else if (serializedData["lightType"] == "spot") toWorkOn->SetLightType(LightType::SPOT);
			else toWorkOn->SetLightType(LightType::POINT);
		}

		if (!serializedData["spotlightInnerAngle"].is_null()) toWorkOn->SetSpotlightInnerAngle(serializedData["spotlightInnerAngle"]);
		if (!serializedData["spotlightOuterAngle"].is_null()) toWorkOn->SetSpotlightOuterAngle(serializedData["spotlightOuterAngle"]);
		if (!serializedData["linearAttenuation"].is_null()) toWorkOn->SetLinearAttenuation(serializedData["linearAttenuation"]);
		if (!serializedData["quadraticAttenuation"].is_null()) toWorkOn->SetQuadraticAttenuation(serializedData["quadraticAttenuation"]);

		// Return toWorkOn
		return owner;
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
	void LightNut::SetLightType(LightType newType)
	{
		type = newType;
	}

	LightNut::LightType LightNut::GetLightType()
	{
		return type;
	}
	void LightNut::SetSpotlightInnerAngle(float angle)
	{
		spotInnerAngle = angle;
	}
	void LightNut::SetSpotlightOuterAngle(float angle)
	{
		spotOuterAngle = angle;
	}
	float LightNut::GetSpotlightInnerAngle()
	{
		return spotInnerAngle;
	}
	float LightNut::GetSpotlightOuterAngle()
	{
		return spotOuterAngle;
	}
	void LightNut::SetLinearAttenuation(float linearAttenuation)
	{
		this->linearAttenuation = linearAttenuation;
	}
	void LightNut::SetQuadraticAttenuation(float quadraticAttenuation)
	{
		this->quadraticAttenuation = quadraticAttenuation;
	}
	float LightNut::GetLinearAttenuation()
	{
		return linearAttenuation;
	}
	float LightNut::GetQuadraticAttenuation()
	{
		return quadraticAttenuation;
	}
}