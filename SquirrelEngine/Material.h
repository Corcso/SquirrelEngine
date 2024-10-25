#pragma once
#include "PCH.h"
#include "Math.h"
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

#ifdef DX11
        struct MaterialDX11Data {
            DirectX::XMFLOAT3 diffuseColor;
            DirectX::XMFLOAT3 specularColor;

            // Padding required to prevent wrapping
            uint32_t padding_0;

            unsigned int specularity;
            float smoothness;

            // Padding requried for 16 byte alignment.
            uint64_t padding_1;
        };
#endif // DX11


    private:
        Vec3 diffuseColor;
        Vec3 specularColor;
        unsigned int specularity;
        float smoothness;
    };
    
}

