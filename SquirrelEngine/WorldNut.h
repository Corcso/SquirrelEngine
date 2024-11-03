#pragma once
#include "Nut.h"
#include "Math.h"
namespace SQ {
    class WorldNut :
        public Nut
    {
    public:

        WorldNut();

        void SetPosition(Vec3 position);
        void SetEulerAngles(Vec3 eulerRotation);
        void SetScale(Vec3 scale);

        Vec3 GetPosition();
        Vec3 GetEulerAngles();
        Vec3 GetScale();

        Mat4 GetSRTWorldMatrix();

        virtual ~WorldNut() override {}
    private:
        Vec3 position;
        Vec3 eulerRotation;
        Vec3 scale;

        Mat4 SRTWorldMatrix;
        void RecalculateWorldMatrices();
    };
}