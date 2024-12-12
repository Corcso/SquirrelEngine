#pragma once
#include "WorldNut.h"
namespace SQ {
    class LightNut :
        public WorldNut
    {
    public:
        static Nut* Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        void SetDiffuseColor(Vec3 diffuseColor);
        void SetAmbientColor(Vec3 ambientColor);

        void SetIntensity(float intensity);
        void SetAmbientIntensity(float ambientIntensity);

        Vec3 GetDiffuseColor();
        Vec3 GetAmbientColor();
        float GetIntensity();
        float GetAmbientIntensity();

        enum class LightType
        {
            DIRECTIONAL, POINT, SPOT
        };

        void SetLightType(LightType newType);
        LightType GetLightType();

        void SetSpotlightInnerAngle(float angle);
        void SetSpotlightOuterAngle(float angle);
        float GetSpotlightInnerAngle();
        float GetSpotlightOuterAngle();

        virtual ~LightNut() {}

    private:
        Vec3 diffuseColor;
        Vec3 ambientColor;

        float intensity;
        float ambientIntensity;

        LightType type;

        float spotInnerAngle; 
        float spotOuterAngle;
    };
}