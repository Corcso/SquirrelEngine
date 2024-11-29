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

        virtual ~LightNut() {}

    private:
        Vec3 diffuseColor;
        Vec3 ambientColor;

        float intensity;
        float ambientIntensity;
    };
}