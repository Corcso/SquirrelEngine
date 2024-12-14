#include "PCH.h"
#include "WorldNut.h"
#include "Services.h"
namespace SQ {
    WorldNut::WorldNut()
    {
        position = V3(0, 0, 0);
        eulerRotation = V3(0, 0, 0);
        scale = V3(1, 1, 1);
        rotation = QFromAxisAngle_LH(V3(1, 0, 0), 0);

        SRTWorldMatrixGlobal = SRTWorldMatrixLocal = SRTWorldMatrixParent = Scale(V3(1, 1, 1));

        RecalculateWorldMatrices();
        RecalculateLocalDirections();
        
    }

    UniquePoolPtr<Nut> WorldNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        WorldNut* toWorkOn = dynamic_cast<WorldNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        UniquePoolPtr<Nut> owner;
        if (deserializeInto == nullptr) {
            UniquePoolPtr<WorldNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<WorldNut>();
            toWorkOn = instance.get();
            owner = instance.StaticUniquePoolPtrCast<Nut>();
            deserializeInto = owner.get();
        }
        // Call parent deserialise, passing in our toWorkOn.
        Nut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 
        if (!serializedData["position"].is_null()) toWorkOn->SetPosition(V3(serializedData["position"][0], serializedData["position"][1], serializedData["position"][2]));
        if (!serializedData["eulerAngles"].is_null()) toWorkOn->SetEulerAngles(V3(serializedData["eulerAngles"][0], serializedData["eulerAngles"][1], serializedData["eulerAngles"][2]));
        if (!serializedData["scale"].is_null()) toWorkOn->SetScale(V3(serializedData["scale"][0], serializedData["scale"][1], serializedData["scale"][2]));

        // Return toWorkOn
        return owner;
    }

    void WorldNut::SetPosition(Vec3 position)
    {
        this->position = position;
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetEulerAngles(Vec3 eulerRotation)
    {
        //this->eulerRotation = eulerRotation;

        rotation = MulQ(MulQ(QFromAxisAngle_LH(V3(1, 0, 0), eulerRotation.X), QFromAxisAngle_LH(V3(0, 1, 0), eulerRotation.Y)), QFromAxisAngle_LH(V3(0, 0, 1), eulerRotation.Z));

        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetRotation(Quat quaternionRotation)
    {
        rotation = quaternionRotation;
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetScale(Vec3 scale)
    {
        this->scale = scale;
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    Vec3 WorldNut::GetPosition()
    {
        return position;
    }

  /*  Vec3 WorldNut::GetEulerAngles()
    {
        return eulerRotation;
    }*/

    Vec3 WorldNut::GetGlobalPosition()
    {
        return (SRTWorldMatrixGlobal * V4(0, 0, 0, 1)).XYZ;
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

    Vec3 WorldNut::GetLocalForward()
    {
        return localForward;
    }

    Vec3 WorldNut::GetLocalRight()
    {
        return localRight;
    }

    Vec3 WorldNut::GetLocalUp()
    {
        return localUp;
    }

    void WorldNut::RotateGlobalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(1, 0, 0), rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateGlobalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 1, 0), rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateGlobalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 0, 1), rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(right, rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(up, rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(forward, rad), rotation);
        //RecalculateLocalDirections();
        //RecalculateWorldMatrices();
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateSuperLocalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(localRight, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateSuperLocalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(localUp, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateSuperLocalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(localForward, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    /*void WorldNut::RotateRelativeGlobalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(relativeGlobalRight, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateRelativeGlobalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(relativeGlobalUp, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateRelativeGlobalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(relativeGlobalRight, rad), rotation);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }*/

    Mat4 SQ::WorldNut::GetLocalSRTWorldMatrix()
    {
        return SRTWorldMatrixLocal;
    }

    Mat4 WorldNut::GetGlobalSRTWorldMatrix()
    {
        return SRTWorldMatrixGlobal;
    }

    Mat4 WorldNut::GetParentGlobalSRTWorldMatrix()
    {
        return SRTWorldMatrixParent;
    }

    void WorldNut::NewChildAdded(bool myChild)
    {
        UpdateTransforms(this, SRTWorldMatrixParent);

        Nut::NewChildAdded(myChild);
    }

    void WorldNut::RecalculateLocalDirections()
    {
        //Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));
        Mat4 rotationMatrix = QToM4(rotation);
        
        forward = NormV3((SRTWorldMatrixGlobal * V4(0, 0, 1, 0)).XYZ);
        right = NormV3((SRTWorldMatrixGlobal * V4(1, 0, 0, 0)).XYZ);
        up = NormV3((SRTWorldMatrixGlobal * V4(0, 1, 0, 0)).XYZ);

        localForward = (rotationMatrix * V4(0, 0, 1, 1)).XYZ;
        localRight = (rotationMatrix * V4(1, 0, 0, 1)).XYZ;
        localUp = (rotationMatrix * V4(0, 1, 0, 1)).XYZ;

        /*relativeGlobalForward = NormV3((SRTWorldMatrixParent * V4(0, 0, 1, 0)).XYZ);
        relativeGlobalRight = NormV3((SRTWorldMatrixParent * V4(1, 0, 0, 0)).XYZ);
        relativeGlobalUp = NormV3((SRTWorldMatrixParent * V4(0, 1, 0, 0)).XYZ);*/
    }

    void WorldNut::RecalculateWorldMatrices()
    {
        Mat4 translateMatrix = Translate(position);
        Mat4 scaleMatrix = Scale(scale);
        //Mat4 rotationMatrix = Rotate_LH(eulerRotation.X, V3(1, 0, 0)) * Rotate_LH(eulerRotation.Y, V3(0, 1, 0)) * Rotate_LH(eulerRotation.Z, V3(0, 0, 1));
        Mat4 rotationMatrix = QToM4(rotation);

        SRTWorldMatrixLocal = translateMatrix * rotationMatrix * scaleMatrix;
        SRTWorldMatrixGlobal = SRTWorldMatrixParent * SRTWorldMatrixLocal;
    }
    void WorldNut::UpdateTransforms(Nut* nut, Mat4 WorldMatrixSoFar)
    {
        // Check if this nut is a world nut
        WorldNut* worldCast = dynamic_cast<WorldNut*>(nut);
        
        // If so, update the parent matrix
        // Recalculate world matrices and local directions
        // And set the parent matrix to be for the child nodes to the global world matrix of this node. 
        if (worldCast != nullptr) {
            worldCast->SRTWorldMatrixParent = WorldMatrixSoFar;
            worldCast->RecalculateWorldMatrices();
            worldCast->RecalculateLocalDirections();
            WorldMatrixSoFar = worldCast->GetGlobalSRTWorldMatrix();
        }

        // Call this for all children too
        unsigned int childCount = nut->GetChildCount();
        for (unsigned int c = 0; c < childCount; ++c) {
            UpdateTransforms(nut->GetNthChild(c), WorldMatrixSoFar);
        }
    }
}