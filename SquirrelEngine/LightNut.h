#pragma once
#include "WorldNut.h"
namespace SQ {
    /// <summary>
    /// Light nut, used for scene lighting.
    /// </summary>
    class LightNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        // Make sure to call base class ready update and late update
        virtual void Ready() override { WorldNut::Ready(); }
        virtual void Update() override { WorldNut::Update(); }
        virtual void LateUpdate() override { WorldNut::LateUpdate(); }

        /// <summary>
        /// Set diffuse color of light, this is the "light color"
        /// </summary>
        /// <param name="diffuseColor">Diffuse Color Vec3</param>
        void SetDiffuseColor(Vec3 diffuseColor);

        /// <summary>
        /// <para>Set ambient color of light, typically the same as diffuse</para>
        /// <para>You might want a different ambient e.g. blue ambient for a sun, as the sky is blue. </para>
        /// </summary>
        /// <param name="ambientColor"></param>
        void SetAmbientColor(Vec3 ambientColor);

        /// <summary>
        /// Sets intensity which is multiplied by the light power. 
        /// </summary>
        /// <param name="intensity">Intensity</param>
        void SetIntensity(float intensity);

        /// <summary>
        /// Set ambient intensity, this is the intensity ambient light will use. Typically a low number. 
        /// </summary>
        /// <param name="ambientIntensity">Ambient Intensity</param>
        void SetAmbientIntensity(float ambientIntensity);

        /// <summary>
        /// Get diffuse color
        /// </summary>
        /// <returns>Diffuse Color</returns>
        Vec3 GetDiffuseColor();

        /// <summary>
        /// Get ambient color
        /// </summary>
        /// <returns>Ambient color</returns>
        Vec3 GetAmbientColor();

        /// <summary>
        /// Get Intensity
        /// </summary>
        /// <returns>Intensity</returns>
        float GetIntensity();

        /// <summary>
        /// Get Ambient Intensity
        /// </summary>
        /// <returns>Ambient Intensity</returns>
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

        /// <para>Used to get the light type, there are 3 types</para>
        /// <para>DIRECTIONAL, an infinitly far away light so that the rays of light are running paralell, used for things like the sun.</para>
        /// <para>POINT, a light which has a position and radiates in all directions, like a lightbulb</para>
        /// <para>SPOT, a light which has a position, direction and cutoff angles, like a spotlight</para>
        /// <returns>Light Type</returns>
        LightType GetLightType();

        /// <summary>
        /// <para>Set the spotlight inner cutoff angle. </para>
        /// <para>Spotlight is full intensity to here then starts to fade</para>
        /// </summary>
        /// <param name="angle">Spotlight inner cutoff angle</param>
        void SetSpotlightInnerAngle(float angle);

        /// <summary>
        /// <para>Set the spotlight outer cutoff angle. </para>
        /// <para>Spotlight is zero intensity beyond here</para>
        /// </summary>
        /// <param name="angle">Spotlight outer cutoff angle</param>
        void SetSpotlightOuterAngle(float angle);

        /// <summary>
        /// Gets the spotlight inner cutoff angle
        /// </summary>
        /// <returns>Spotlight inner cutoff angle</returns>
        float GetSpotlightInnerAngle();

        /// <summary>
        /// Gets the spotlight outer cutoff angle
        /// </summary>
        /// <returns>Spotlight outer cutoff angle</returns>
        float GetSpotlightOuterAngle();

        /// <summary>
        /// <para>Set linear attenuation. </para>
        /// <para>This is how much the light intensity will drop with linear distance</para>
        /// <para>There is no constant attenuation as intensity provides the same effect</para>
        /// </summary>
        /// <param name="linearAttenuation">Linear Attenuation</param>
        void SetLinearAttenuation(float linearAttenuation);

        /// <summary>
        /// <para>Set quadratic attenuation. </para>
        /// <para>This is how much the light intensity will drop with d^2 distance</para>
        /// <para>There is no constant attenuation as intensity provides the same effect</para>
        /// </summary>
        /// <param name="quadraticAttenuation">Quadratic Attenuation</param>
        void SetQuadraticAttenuation(float quadraticAttenuation);

        /// <summary>
        /// Gets the linear attenuation
        /// </summary>
        /// <returns>Linear attenuation</returns>
        float GetLinearAttenuation();

        /// <summary>
        /// Gets the quadratic attenuation
        /// </summary>
        /// <returns>Quadratic attenuation</returns>
        float GetQuadraticAttenuation();

        virtual ~LightNut() {}

    private:
        Vec3 diffuseColor;
        Vec3 ambientColor;

        float intensity;
        float ambientIntensity;

        LightType type;

        float spotInnerAngle; 
        float spotOuterAngle;
        // No need for constant attenuation, this can be edited by changing intensity. 
        float linearAttenuation;
        float quadraticAttenuation;
    };
}