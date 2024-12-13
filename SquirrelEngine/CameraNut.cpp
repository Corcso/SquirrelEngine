#include "PCH.h"
#include "CameraNut.h"
namespace SQ {
    CameraNut::CameraNut()
    {
        FOV = 70;
        isActiveCamera = false;
    }

    Nut* CameraNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        CameraNut* toWorkOn = dynamic_cast<CameraNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        UniquePoolPtr<Nut> owner;
        if (deserializeInto == nullptr) {
            //owner = DynamicUniquePoolPtrCast<Nut, CameraNut>(Services::GetPoolAllocationService()->MakeUniquePoolPtr<CameraNut>());
            deserializeInto = owner.get();
        }
        // Call parent deserialise, passing in our toWorkOn.
        WorldNut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 
        if(!serializedData["fov"].is_null()) toWorkOn->SetFov(serializedData["fov"]);
        if (!serializedData["isActive"].is_null()) {
            if (serializedData["isActive"]) toWorkOn->SetActiveCamera();
            else toWorkOn->SetDeactivatedCamera();
        }

        // Return owner
        return owner.get();
    }
    Mat4 SQ::CameraNut::GetViewMatrix()
    {
        return LookAt_LH(GetGlobalPosition(), GetGlobalPosition() + GetForward(), GetUp());
    }

    void CameraNut::SetFov(float FOV)
    {
        this->FOV = FOV;
    }

    float CameraNut::GetFov()
    {
        return FOV;
    }

    bool SQ::CameraNut::IsActiveCamera()
    {
        return isActiveCamera;
    }

    void SQ::CameraNut::SetActiveCamera()
    {
        isActiveCamera = true;
    }

    void SQ::CameraNut::SetDeactivatedCamera()
    {
        isActiveCamera = false;
    }
}