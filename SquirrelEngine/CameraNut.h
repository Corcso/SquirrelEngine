#pragma once
#include "WorldNut.h"
namespace SQ {
    class CameraNut :
        public WorldNut
    {
    public:
        Mat4 GetViewMatrix();
        void SetFov(float FOV);
        float GetFov();

        bool IsActiveCamera();

        void SetActiveCamera();
        void SetDeactivatedCamera();

    private:
        bool isActiveCamera;
        float FOV;
    };
}