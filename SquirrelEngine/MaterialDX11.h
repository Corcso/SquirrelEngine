#pragma once

#ifdef DX11
#include "SquirrelEnginePI.h"
#include "Material.h"
namespace SQ {
    class MaterialDX11 :
        public Material
    {
    public:
        virtual void SetDiffuseColor(Vec3 diffuseColor) final;
        virtual void SetSpecularColor(Vec3 specularColor) final;
        virtual void SetSpecularity(unsigned int specularity) final;
        virtual void SetSmoothness(float smoothness) final;

        virtual Vec3 GetDiffuseColor() final;
        virtual Vec3 GetSpecularColor() final;
        virtual unsigned int GetSpecularity() final;
        virtual float GetSmoothness() final;

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

        MaterialDX11Data GetBufferData();

    private:
        MaterialDX11Data bufferData;
    };
}
#endif // DX11