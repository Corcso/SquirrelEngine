#pragma once
#include "PCH.h"
#include "SQMath.h"
#include "Resource.h"

namespace SQ {
    /// <summary>
    /// Material applied to mesh nuts
    /// </summary>
    class Material :
        public Resource
    {
    public:
        /// <summary>
        /// Loads a material and returns a pointer to it. 
        /// </summary>
        /// <param name="path">Path of the material .mat file</param>
        /// <returns>A pointer to the new material</returns>
        static Material* Load(std::string path);

        /// <summary>
        /// Set diffuse color, this is the color of the thing. 
        /// </summary>
        /// <param name="diffuseColor">Diffuse color</param>
        virtual void SetDiffuseColor(Vec3 diffuseColor) = 0;

        /// <summary>
        /// Set specular color, this is typically white. 
        /// </summary>
        /// <param name="specularColor">Specular color</param>
        virtual void SetSpecularColor(Vec3 specularColor) = 0;

        /// <summary>
        /// Set specularity, larger the specularity, the smaller and sharper the highlight. 
        /// Must be 1 or above. 
        /// </summary>
        /// <param name="specularity">Specularity</param>
        virtual void SetSpecularity(unsigned int specularity) = 0;

        /// <summary>
        /// Set the smoothness, 0 is matte, 1 is shiny. 
        /// </summary>
        /// <param name="smoothness"></param>
        virtual void SetSmoothness(float smoothness) = 0;

        /// <summary>
        /// Get the diffuse color.
        /// </summary>
        /// <returns>Diffuse color</returns>
        virtual Vec3 GetDiffuseColor() = 0;

        /// <summary>
        /// Get the specular color.
        /// </summary>
        /// <returns>Specular color</returns>
        virtual Vec3 GetSpecularColor() = 0;

        /// <summary>
        /// Get the specularity. 
        /// </summary>
        /// <returns>Specularity</returns>
        virtual unsigned int GetSpecularity() = 0;

        /// <summary>
        /// Get the smoothness
        /// </summary>
        /// <returns>Smoothness</returns>
        virtual float GetSmoothness() = 0;

    };
    
}

