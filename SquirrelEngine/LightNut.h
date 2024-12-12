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

        /// <summary>
        /// <para>Used for the light type, there are 3 types</para>
        /// <para>DIRECTIONAL, an infinitly far away light so that the rays of light are running paralell, used for things like the sun.</para>
        /// <para>POINT, a light which has a position and radiates in all directions, like a lightbulb</para>
        /// <para>SPOT, a light which has a position, direction and cutoff angles, like a spotlight</para>
        /// </summary>
        enum class LightType
        {
            DIRECTIONAL, POINT, SPOT
        };

        /// <summary>
        /// <para>Used to set the light type, there are 3 types</para>
        /// <para>DIRECTIONAL, an infinitly far away light so that the rays of light are running paralell, used for things like the sun.</para>
        /// <para>POINT, a light which has a position and radiates in all directions, like a lightbulb</para>
        /// <para>SPOT, a light which has a position, direction and cutoff angles, like a spotlight</para>
        /// </summary>
        /// <param name="newType">Light type</param>
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