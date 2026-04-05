#pragma once

#ifdef VULKAN
#include "Material.h"

namespace SQ {
    class MaterialVulkan :
        public StandardMaterial
    {
    public:
        /// <summary>
        /// UNUSED Loads a material and returns a pointer to it. 
        /// </summary>
        /// <param name="path">Path of the material .mat file</param>
        /// <returns>A pointer to the new material</returns>
        static StandardMaterial* Load(std::string path);

        /// <summary>
        /// StandardMaterial data struct in a GPU friendly format
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
        /// <returns>StandardMaterial buffer data</returns>
        MaterialVulkanData* GetBufferData();

    private:
        MaterialVulkanData bufferData;
    };
}

#endif // VULKAN