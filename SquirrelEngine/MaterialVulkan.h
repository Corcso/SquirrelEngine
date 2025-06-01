#pragma once

#ifdef VULKAN
#include "Material.h"

namespace SQ {
    class MaterialVulkan :
        public Material
    {
    public:
        /// <summary>
        /// Loads a material and returns a pointer to it. 
        /// </summary>
        /// <param name="path">Path of the material .mat file</param>
        /// <returns>A pointer to the new material</returns>
        static Material* Load(std::string path);

        // Overrides for properties

        virtual void SetDiffuseColor(Vec3 diffuseColor) final;
        virtual void SetSpecularColor(Vec3 specularColor) final;
        virtual void SetSpecularity(unsigned int specularity) final;
        virtual void SetSmoothness(float smoothness) final;

        virtual Vec3 GetDiffuseColor() final;
        virtual Vec3 GetSpecularColor() final;
        virtual unsigned int GetSpecularity() final;
        virtual float GetSmoothness() final;

        /// <summary>
        /// Material data struct in a GPU friendly format
        /// </summary>
        struct MaterialVulkanData {
            alignas(16) Vec3 diffuseColor;

            // Padding required
            

            alignas(16) Vec3 specularColor;

            alignas(4) unsigned int specularity;
            alignas(4) float smoothness;
        };

        /// <summary>
        /// Returns the buffer data in the correct GPU format for the constant buffer. 
        /// </summary>
        /// <returns>Material buffer data</returns>
        MaterialVulkanData* GetBufferData();

    private:
        MaterialVulkanData bufferData;
    };
}

#endif // VULKAN