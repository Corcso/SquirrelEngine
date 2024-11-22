#pragma once
#include "Nut.h"
#include "Math.h"
namespace SQ {
    class WorldNut :
        public Nut
    {
    public:

        WorldNut();

        /// <summary>
        /// Sets the Local Position of the WorldNut
        /// </summary>
        /// <param name="position">Local Position</param>
        void SetPosition(Vec3 position);

        /// <summary>
        /// Sets the Euler Angle Rotation of the WorldNut
        /// Uses global pitch yaw and roll axes
        /// </summary>
        /// <param name="eulerRotation">Euler Angle Rotation</param>
        void SetEulerAngles(Vec3 eulerRotation);

        /// <summary>
        /// Sets the Local Scale of the WorldNut
        /// </summary>
        /// <param name="scale">Local Scale</param>
        void SetScale(Vec3 scale);

        /// <summary>
        /// Returns the Local Position of the WorldNut
        /// </summary>
        /// <returns>Local Position</returns>
        Vec3 GetPosition();
        //Vec3 GetEulerAngles();

        /// <summary>
        /// Returns the Local Scale of the WorldNut
        /// </summary>
        /// <returns>Local Scale</returns>
        Vec3 GetScale();

        /// <summary>
        /// Returns the Forward Vector for the WorldNut
        /// </summary>
        /// <returns>Forward Vector</returns>
        Vec3 GetForward();

        /// <summary>
        /// Returns the Right Vector for the WorldNut
        /// </summary>
        /// <returns>Right Vector</returns>
        Vec3 GetRight();

        /// <summary>
        /// Returns the Up Vector for the WorldNut
        /// </summary>
        /// <returns>Up Vector</returns>
        Vec3 GetUp();

        /// <summary>
        /// Rotates by WorldNut by the global X Axis (1, 0, 0)
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateGlobalX(float rad);

        /// <summary>
        /// Rotates by WorldNut by the global Y Axis (0, 1, 0)
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateGlobalY(float rad);

        /// <summary>
        /// Rotates by WorldNut by the global Z Axis (0, 0, 1)
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateGlobalZ(float rad);

        /// <summary>
        /// Rotates by WorldNut by the local X Axis, it's right vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateLocalX(float rad);

        /// <summary>
        /// Rotates by WorldNut by the local X Axis, it's up vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateLocalY(float rad);

        /// <summary>
        /// Rotates by WorldNut by the local X Axis, it's forwards vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateLocalZ(float rad);

        /// <summary>
        /// Returns the World Matrix of the WorldNut in Scale then Rotation then Translation Format. 
        /// </summary>
        /// <returns></returns>
        Mat4 GetSRTWorldMatrix();

        virtual ~WorldNut() override {}
    private:
        Vec3 position;
        Vec3 eulerRotation;
        Quat rotation;
        Vec3 scale;

        Vec3 forward;
        Vec3 up;
        Vec3 right;

        /// <summary>
        /// Recalculates the local directions: right, up and forwards. 
        /// </summary>
        void RecalculateLocalDirections();

        Mat4 SRTWorldMatrix;
        /// <summary>
        /// Recalculates all world matrices.
        /// </summary>
        void RecalculateWorldMatrices();
    };
}