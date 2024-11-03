#include "PCH.h"
#include "WorldNut.h"
namespace SQ {
    void WorldNut::SetPosition(Vec3 position)
    {
        this->position = position;
        RecalculateWorldMatrices();
    }

    void WorldNut::SetEulerAngles(Vec3 eulerRotation)
    {
        this->eulerRotation = eulerRotation;
        RecalculateWorldMatrices();
    }

    void WorldNut::SetScale(Vec3 scale)
    {
        this->scale = scale;
        RecalculateWorldMatrices();
    }

    Vec3 WorldNut::GetPosition()
    {
        return position;
    }

    Vec3 WorldNut::GetEulerAngles()
    {
        return eulerRotation;
    }

    Vec3 WorldNut::GetScale()
    {
        return scale;
    }

    Mat4 SQ::WorldNut::GetSRTWorldMatrix()
    {
        return SRTWorldMatrix;
    }

    void WorldNut::RecalculateWorldMatrices()
    {
        Mat4 translateMatrix = Translate(position);
        Mat4 scaleMatrix = Scale(scale);
        Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));

        SRTWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
    }
}