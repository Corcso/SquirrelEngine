#include "PCH.h"
#include "WorldNut.h"
namespace SQ {
    WorldNut::WorldNut()
    {
        position = V3(0, 0, 0);
        eulerRotation = V3(0, 0, 0);
        scale = V3(1, 1, 1);
        rotation = QFromAxisAngle_LH(V3(1, 0, 0), 0);
        RecalculateWorldMatrices();
        RecalculateLocalDirections();
        
    }

    Nut* WorldNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        WorldNut* toWorkOn = dynamic_cast<WorldNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        if (toWorkOn == nullptr) toWorkOn = new WorldNut();
        // Call parent deserialise, passing in our toWorkOn.
        Nut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 
        if (!serializedData["position"].is_null()) toWorkOn->SetPosition(V3(serializedData["position"][0], serializedData["position"][1], serializedData["position"][2]));
        if (!serializedData["eulerAngles"].is_null()) toWorkOn->SetEulerAngles(V3(serializedData["eulerAngles"][0], serializedData["eulerAngles"][1], serializedData["eulerAngles"][2]));
        if (!serializedData["scale"].is_null()) toWorkOn->SetScale(V3(serializedData["scale"][0], serializedData["scale"][1], serializedData["scale"][2]));

        // Return toWorkOn
        return toWorkOn;
    }

    void WorldNut::SetPosition(Vec3 position)
    {
        this->position = position;
        RecalculateWorldMatrices();
    }

    void WorldNut::SetEulerAngles(Vec3 eulerRotation)
    {
        //this->eulerRotation = eulerRotation;

        rotation = MulQ(MulQ(QFromAxisAngle_LH(V3(1, 0, 0), eulerRotation.X), QFromAxisAngle_LH(V3(0, 1, 0), eulerRotation.Y)), QFromAxisAngle_LH(V3(0, 0, 1), eulerRotation.Z));

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

  /*  Vec3 WorldNut::GetEulerAngles()
    {
        return eulerRotation;
    }*/

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

    void WorldNut::RotateGlobalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(1, 0, 0), rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    void WorldNut::RotateGlobalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 1, 0), rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    void WorldNut::RotateGlobalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 0, 1), rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    void WorldNut::RotateLocalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(right, rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    void WorldNut::RotateLocalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(up, rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    void WorldNut::RotateLocalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(forward, rad), rotation);
        RecalculateLocalDirections();
        RecalculateWorldMatrices();
    }

    Mat4 SQ::WorldNut::GetSRTWorldMatrix()
    {
        return SRTWorldMatrix;
    }

    void WorldNut::RecalculateLocalDirections()
    {
        //Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));
        Mat4 rotationMatrix = QToM4(rotation);
        
        forward = (rotationMatrix * V4(0, 0, 1, 1)).XYZ;
        right = (rotationMatrix * V4(1, 0, 0, 1)).XYZ;
        up = (rotationMatrix * V4(0, 1, 0, 1)).XYZ;
    }

    void WorldNut::RecalculateWorldMatrices()
    {
        Mat4 translateMatrix = Translate(position);
        Mat4 scaleMatrix = Scale(scale);
        //Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));
        Mat4 rotationMatrix = QToM4(rotation);

        SRTWorldMatrix = scaleMatrix * rotationMatrix * translateMatrix;
    }
}