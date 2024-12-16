#pragma once

#ifdef DX11
#include "SquirrelEnginePI.h"
#include "Material.h"
namespace SQ {
    class MaterialDX11 :
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
        struct MaterialDX11Data {
            DirectX::XMFLOAT3 diffuseColor;

            // Padding required
            uint32_t padding_0;

            DirectX::XMFLOAT3 specularColor;
            unsigned int specularity;

            float smoothness;

            // Padding requried for 16 byte alignment.
            uint32_t padding_1;
            uint32_t padding_2;
            uint32_t padding_3;
        };

        /// <summary>
        /// Returns the buffer data in the correct GPU format for the constant buffer. 
        /// </summary>
        /// <returns>Material buffer data</returns>
        MaterialDX11Data* GetBufferData();

    private:
        MaterialDX11Data bufferData;
    };
}
#endif // DX11