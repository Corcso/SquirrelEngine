#include "PCH.h"
#include "CameraNut.h"
namespace SQ {
    Mat4 SQ::CameraNut::GetViewMatrix()
    {
        return LookAt_LH(GetPosition(), V3(0, 0, 0), V3(0, 1, 0));
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