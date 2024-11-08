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

        Vec3 GetForward();
        Vec3 GetRight();
        Vec3 GetUp();

        Mat4 GetSRTWorldMatrix();

        virtual ~WorldNut() override {}
    private:
        Vec3 position;
        Vec3 eulerRotation;
        Vec3 scale;

        Vec3 forward;
        Vec3 up;
        Vec3 right;
        void RecalculateLocalDirections();

        Mat4 SRTWorldMatrix;
        void RecalculateWorldMatrices();
    };
}