#pragma once
#include "PCH.h"
#include "SQMath.h"
#include "Resource.h"

namespace SQ {
    /// <summary>
    /// StandardMaterial applied to mesh nuts
    /// </summary>
    class StandardMaterial :
        public Resource
    {
    public:
        /// <summary>
        /// Loads a material and returns a pointer to it. 
        /// </summary>
        /// <param name="path">Path of the material .mat file</param>
        /// <returns>A pointer to the new material</returns>
        static StandardMaterial* Load(std::string path);

        static void Save(const StandardMaterial& resource, std::string path);

        /// <summary>
        /// Set diffuse color, this is the color of the thing. 
        /// </summary>
        /// <param name="diffuseColor">Diffuse color</param>
        virtual void SetDiffuseColor(Vec3 diffuseColor);

        /// <summary>
        /// Set specular color, this is typically white. 
        /// </summary>
        /// <param name="specularColor">Specular color</param>
        virtual void SetSpecularColor(Vec3 specularColor);

        /// <summary>
        /// Set specularity, larger the specularity, the smaller and sharper the highlight. 
        /// Must be 1 or above. 
        /// </summary>
        /// <param name="specularity">Specularity</param>
        virtual void SetSpecularity(unsigned int specularity);

        /// <summary>
        /// Set the smoothness, 0 is matte, 1 is shiny. 
        /// </summary>
        /// <param name="smoothness"></param>
        virtual void SetSmoothness(float smoothness);

        /// <summary>
        /// Get the diffuse color.
        /// </summary>
        /// <returns>Diffuse color</returns>
        virtual Vec3 GetDiffuseColor();

        /// <summary>
        /// Get the specular color.
        /// </summary>
        /// <returns>Specular color</returns>
        virtual Vec3 GetSpecularColor();

        /// <summary>
        /// Get the specularity. 
        /// </summary>
        /// <returns>Specularity</returns>
        virtual unsigned int GetSpecularity();

        /// <summary>
        /// Get the smoothness
        /// </summary>
        /// <returns>Smoothness</returns>
        virtual float GetSmoothness();

        virtual void ImGuiRenderMyInspector() override;

    protected:
        Vec3 diffuseColor;
        Vec3 specularColor;

        unsigned int specularity;
        float smoothness;
    };
    
}

