#include "PCH.h"
#include "WorldNut.h"
namespace SQ {
    WorldNut::WorldNut()
    {
        position = V3(0, 0, 0);
        eulerRotation = V3(0, 0, 0);
        scale = V3(1, 1, 1);
        RecalculateWorldMatrices();
        RecalculateLocalDirections();
    }

    void WorldNut::SetPosition(Vec3 position)
    {
        this->position = position;
        RecalculateWorldMatrices();
    }

    void WorldNut::SetEulerAngles(Vec3 eulerRotation)
    {
        this->eulerRotation = eulerRotation;
        RecalculateWorldMatrices();
        RecalculateLocalDirections();
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

    Vec3 WorldNut::GetForward()
    {
        return forward;
    }

    Vec3 WorldNut::GetRight()
    {
        return right;
    }

    Vec3 WorldNut::GetUp()
    {
        return up;
    }

    Mat4 SQ::WorldNut::GetSRTWorldMatrix()
    {
        return SRTWorldMatrix;
    }

    void WorldNut::RecalculateLocalDirections()
    {
        Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));

        forward = (rotationMatrix * V4(0, 0, 1, 1)).XYZ;
        right = (rotationMatrix * V4(1, 0, 0, 1)).XYZ;
        up = (rotationMatrix * V4(0, 1, 0, 1)).XYZ;
    }

    void WorldNut::RecalculateWorldMatrices()
    {
        Mat4 translateMatrix = Translate(position);
        Mat4 scaleMatrix = Scale(scale);
        Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));

        SRTWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
    }
}