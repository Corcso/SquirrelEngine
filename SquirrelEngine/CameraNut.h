#pragma once
#include "WorldNut.h"
namespace SQ {
    class CameraNut :
        public WorldNut
    {
    public:
        CameraNut();

        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        /// <summary>
        /// <para>Returns the View Matrix of this camera.</para>
        /// <para>Left Handed</para>
        /// </summary>
        /// <returns>View Matrix of Camera</returns>
        Mat4 GetViewMatrix();

        /// <summary>
        /// Sets the camera's FOV
        /// </summary>
        /// <param name="FOV">Field of view in degrees</param>
        void SetFov(float FOV);

        /// <summary>
        /// Returns the camera's FOV
        /// </summary>
        /// <returns>Field of view in degrees</returns>
        float GetFov();

        /// <summary>
        /// Is the camera active?
        /// </summary>
        /// <returns>True if active, false if not</returns>
        bool IsActiveCamera();

        /// <summary>
        /// Sets the camera to active.
        /// </summary>
        void SetActiveCamera();

        /// <summary>
        /// Sets the camera to not active.
        /// </summary>
        void SetDeactivatedCamera();

    private:
        bool isActiveCamera;
        float FOV;
    };
}