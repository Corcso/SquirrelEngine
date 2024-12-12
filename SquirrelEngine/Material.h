#pragma once
#include "PCH.h"
#include "SQMath.h"
#include "Resource.h"

namespace SQ {
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

        virtual void SetDiffuseColor(Vec3 diffuseColor) = 0;
        virtual void SetSpecularColor(Vec3 specularColor) = 0;
        virtual void SetSpecularity(unsigned int specularity) = 0;
        virtual void SetSmoothness(float smoothness) = 0;

        virtual Vec3 GetDiffuseColor() = 0;
        virtual Vec3 GetSpecularColor() = 0;
        virtual unsigned int GetSpecularity() = 0;
        virtual float GetSmoothness() = 0;

    protected:
        //Vec3 diffuseColor;
        //Vec3 specularColor;
        //unsigned int specularity;
        //float smoothness;
    };
    
}

