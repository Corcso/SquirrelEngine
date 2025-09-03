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
        // We need to follow strict ownership with the pool ptr
        UniquePoolPtr<Nut> owner;
        if (toWorkOn == nullptr) {
            // Get the instance
            UniquePoolPtr<WorldNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<WorldNut>();
            // Set to work on to the instance
            toWorkOn = instance.get();
            // Transfer ownership into owner and static cast to nut base class
            owner = instance.StaticUniquePoolPtrCast<Nut>();
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

    void WorldNut::SetGlobalPosition(Vec3 position)
    {
        Mat4 translationLocally = InvGeneralM4(SRTWorldMatrixParent) * Translate(position);
        this->position = V3(translationLocally.Columns[3].X, translationLocally.Columns[3].Y, translationLocally.Columns[3].Z);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetEulerAngles(Vec3 eulerRotation)
    {
        rotation = MulQ(MulQ(QFromAxisAngle_LH(V3(1, 0, 0), eulerRotation.X), QFromAxisAngle_LH(V3(0, 1, 0), eulerRotation.Y)), QFromAxisAngle_LH(V3(0, 0, 1), eulerRotation.Z));

        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetGlobalEulerAngles(Vec3 eulerRotation)
    {
        Mat4 rotationLocally = InvGeneralM4(SRTWorldMatrixParent) *
            QToM4(MulQ(MulQ(QFromAxisAngle_LH(V3(1, 0, 0), eulerRotation.X), QFromAxisAngle_LH(V3(0, 1, 0), eulerRotation.Y)), QFromAxisAngle_LH(V3(0, 0, 1), eulerRotation.Z)));
        this->rotation = M4ToQ_RH(rotationLocally);
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetGlobalQuaternion(Quat quaternionRotation)
    {
        /*Vec3 tempScale = V3(
            Len(SRTWorldMatrixParent.Columns[0]),
            Len(SRTWorldMatrixParent.Columns[1]),
            Len(SRTWorldMatrixParent.Columns[2]));
        Mat4 rotationOnly = M4D(1);
        rotationOnly[0] = SRTWorldMatrixParent[0] / tempScale.X;
        rotationOnly[1] = SRTWorldMatrixParent[1] / tempScale.Y;
        rotationOnly[2] = SRTWorldMatrixParent[2] / tempScale.Z;
        rotationOnly[3] = V4(0, 0, 0, 1);*/

        

        Mat4 rotationLocally = SRTTransformToRotation(  InvGeneralM4(SRTWorldMatrixParent) * QToM4(quaternionRotation));
        //std::cout << "Interior 1 " << SRTTransformToScale(rotationLocally).X << " " << SRTTransformToScale(rotationLocally).Y << " " << SRTTransformToScale(rotationLocally).Z << " ";
        this->rotation = M4ToQ_RH(rotationLocally);
        //Mat4 test = QToM4(this->rotation);
        //std::cout << "Interior 2 " << SRTTransformToScale(test).X << " " << SRTTransformToScale(test).Y << " " << SRTTransformToScale(test).Z << " ";

        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::SetGlobalScale(Vec3 scale)
    {
        Mat4 translationLocally =  InvGeneralM4(SRTWorldMatrixParent) * Scale(scale);
        /*std::cout << "\nPPINV\n" << SRTWorldMatrixParent[0][0] << " " << SRTWorldMatrixParent[0][1] << " " << SRTWorldMatrixParent[0][2] << " " << SRTWorldMatrixParent[0][3] << "\n";
        std::cout << SRTWorldMatrixParent[1][0] << " " << SRTWorldMatrixParent[1][1] << " " << SRTWorldMatrixParent[1][2] << " " << SRTWorldMatrixParent[1][3] << "\n";
        std::cout << SRTWorldMatrixParent[2][0] << " " << SRTWorldMatrixParent[2][1] << " " << SRTWorldMatrixParent[2][2] << " " << SRTWorldMatrixParent[2][3] << "\n";
        std::cout << SRTWorldMatrixParent[3][0] << " " << SRTWorldMatrixParent[3][1] << " " << SRTWorldMatrixParent[3][2] << " " << SRTWorldMatrixParent[3][3] << "\n";
        Mat4 pinv = InvGeneralM4(SRTWorldMatrixParent);
        std::cout << "\nPINV\n" << pinv[0][0] << " " << pinv[0][1] << " " << pinv[0][2] << " " << pinv[0][3] << "\n";
        std::cout << pinv[1][0] << " " << pinv[1][1] << " " << pinv[1][2] << " " << pinv[1][3] << "\n";
        std::cout << pinv[2][0] << " " << pinv[2][1] << " " << pinv[2][2] << " " << pinv[2][3] << "\n";
        std::cout << pinv[3][0] << " " << pinv[3][1] << " " << pinv[3][2] << " " << pinv[3][3] << "\n";
        std::cout << "\nSCONLYMAT\n" << translationLocally[0][0] << " " << translationLocally[0][1] << " " << translationLocally[0][2] << " " << translationLocally[0][3] << "\n";
        std::cout << translationLocally[1][0] << " " << translationLocally[1][1] << " " << translationLocally[1][2] << " " << translationLocally[1][3] << "\n";
        std::cout << translationLocally[2][0] << " " << translationLocally[2][1] << " " << translationLocally[2][2] << " " << translationLocally[2][3] << "\n";
        std::cout << translationLocally[3][0] << " " << translationLocally[3][1] << " " << translationLocally[3][2] << " " << translationLocally[3][3] << "\n";*/
        this->scale = SRTTransformToScale(translationLocally);
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
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    Vec3 WorldNut::GetPosition()
    {
        return position;
    }

    Quat WorldNut::GetRotation()
    {
        return rotation;
    }

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
      
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateGlobalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 1, 0), rad), rotation);
 
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateGlobalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(V3(0, 0, 1), rad), rotation);
     
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalX(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(right, rad), rotation);

        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalY(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(up, rad), rotation);
      
        UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::RotateLocalZ(float rad)
    {
        rotation = MulQ(QFromAxisAngle_LH(forward, rad), rotation);

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

    void WorldNut::ImGuiRenderMyInspector()
    {
        Vec3 positionBefore = position;
        Quat rotationBefore = rotation;
        Vec3 scaleBefore = scale;

        Nut::ImGuiRenderMyInspector();
        if (ImGui::TreeNodeEx("WorldNut", ImGuiTreeNodeFlags_DefaultOpen)) {
            //ImGui::BeginDisabled();
            ImGui::DragFloat3("Position", reinterpret_cast<float*>(&position));
            ImGui::DragFloat4("Rotation", reinterpret_cast<float*>(&rotation));
            ImGui::DragFloat3("Scale", reinterpret_cast<float*>(&scale));

            ImGui::DragFloat4("WM 1", reinterpret_cast<float*>(&(SRTWorldMatrixGlobal[0])));
            ImGui::DragFloat4("WM 2", reinterpret_cast<float*>(&(SRTWorldMatrixGlobal[1])));
            ImGui::DragFloat4("WM 3", reinterpret_cast<float*>(&(SRTWorldMatrixGlobal[2])));
            ImGui::DragFloat4("WM 4", reinterpret_cast<float*>(&(SRTWorldMatrixGlobal[3])));

            //ImGui::EndDisabled();
            ImGui::TreePop();
        }
        // Update only if needed
        if(position != positionBefore || rotation.X != rotationBefore.X || rotation.Y != rotationBefore.Y || rotation.Z != rotationBefore.Z || rotation.W != rotationBefore.W || scale != scaleBefore) UpdateTransforms(this, SRTWorldMatrixParent);
    }

    void WorldNut::NewChildAdded(bool myChild, Nut* newChild)
    {
        UpdateTransforms(this, SRTWorldMatrixParent);

        Nut::NewChildAdded(myChild, newChild);
    }

    void WorldNut::RecalculateLocalDirections()
    {
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
        Mat4 rotationMatrix = QToM4(rotation);

        SRTWorldMatrixLocal = translateMatrix * rotationMatrix * scaleMatrix;
        //std::cout << "LOCAL " << SRTTransformToScale(SRTWorldMatrixLocal).X << " " << SRTTransformToScale(SRTWorldMatrixLocal).Y << " " << SRTTransformToScale(SRTWorldMatrixLocal).Z << " ";
        SRTWorldMatrixGlobal = SRTWorldMatrixParent * SRTWorldMatrixLocal;

       /* std::cout << "PARE " << SRTTransformToScale(SRTWorldMatrixParent).X << " " << SRTTransformToScale(SRTWorldMatrixParent).Y << " " << SRTTransformToScale(SRTWorldMatrixParent).Z << " ";
        std::cout << "GLOB " << SRTTransformToScale(SRTWorldMatrixGlobal).X << " " << SRTTransformToScale(SRTWorldMatrixGlobal).Y << " " << SRTTransformToScale(SRTWorldMatrixGlobal).Z << " ";
        std::cout << "\nMULTY\n" << SRTWorldMatrixParent[0][0] << " " << SRTWorldMatrixParent[0][1] << " " << SRTWorldMatrixParent[0][2] << " " << SRTWorldMatrixParent[0][3] << " * "
            << SRTWorldMatrixLocal[0][0] << " " << SRTWorldMatrixLocal[0][1] << " " << SRTWorldMatrixLocal[0][2] << " " << SRTWorldMatrixLocal[0][3] << " =\t"
            << SRTWorldMatrixGlobal[0][0] << " " << SRTWorldMatrixGlobal[0][1] << " " << SRTWorldMatrixGlobal[0][2] << " " << SRTWorldMatrixGlobal[0][3] << "\n";
        std::cout << SRTWorldMatrixParent[1][0] << " " << SRTWorldMatrixParent[1][1] << " " << SRTWorldMatrixParent[1][2] << " " << SRTWorldMatrixParent[1][3] << " * "
            << SRTWorldMatrixLocal[1][0] << " " << SRTWorldMatrixLocal[1][1] << " " << SRTWorldMatrixLocal[1][2] << " " << SRTWorldMatrixLocal[1][3] << " =\t"
            << SRTWorldMatrixGlobal[1][0] << " " << SRTWorldMatrixGlobal[1][1] << " " << SRTWorldMatrixGlobal[1][2] << " " << SRTWorldMatrixGlobal[1][3] << "\n";
        std::cout << SRTWorldMatrixParent[2][0] << " " << SRTWorldMatrixParent[2][1] << " " << SRTWorldMatrixParent[2][2] << " " << SRTWorldMatrixParent[2][3] << " * "
            << SRTWorldMatrixLocal[2][0] << " " << SRTWorldMatrixLocal[2][1] << " " << SRTWorldMatrixLocal[2][2] << " " << SRTWorldMatrixLocal[2][3] << " =\t"
            << SRTWorldMatrixGlobal[2][0] << " " << SRTWorldMatrixGlobal[2][1] << " " << SRTWorldMatrixGlobal[2][2] << " " << SRTWorldMatrixGlobal[2][3] << "\n";
        std::cout << SRTWorldMatrixParent[3][0] << " " << SRTWorldMatrixParent[3][1] << " " << SRTWorldMatrixParent[3][2] << " " << SRTWorldMatrixParent[3][3] << " * "
            << SRTWorldMatrixLocal[3][0] << " " << SRTWorldMatrixLocal[3][1] << " " << SRTWorldMatrixLocal[3][2] << " " << SRTWorldMatrixLocal[3][3] << " =\t"
            << SRTWorldMatrixGlobal[3][0] << " " << SRTWorldMatrixGlobal[3][1] << " " << SRTWorldMatrixGlobal[3][2] << " " << SRTWorldMatrixGlobal[3][3] << "\n";*/
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