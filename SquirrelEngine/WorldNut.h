#pragma once
#include "Nut.h"
#include "SQMath.h"
namespace SQ {
    class WorldNut :
        public Nut
    {
    public:

        WorldNut();

        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

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
        /// Returns the Global Position of the WorldNut
        /// </summary>
        /// <returns>Global Position</returns>
        Vec3 GetGlobalPosition();

        /// <summary>
        /// Returns the Local Scale of the WorldNut
        /// </summary>
        /// <returns>Local Scale</returns>
        Vec3 GetScale();

        /// <summary>
        /// Returns the Global Forward Vector for the WorldNut
        /// </summary>
        /// <returns>Forward Vector</returns>
        Vec3 GetForward();

        /// <summary>
        /// Returns the Global Right Vector for the WorldNut
        /// </summary>
        /// <returns>Right Vector</returns>
        Vec3 GetRight();

        /// <summary>
        /// Returns the Global Up Vector for the WorldNut
        /// </summary>
        /// <returns>Up Vector</returns>
        Vec3 GetUp();

        /// <summary>
        /// Returns the Local Forward Vector for the WorldNut
        /// </summary>
        /// <returns>Local Forward Vector</returns>
        Vec3 GetLocalForward();

        /// <summary>
        /// Returns the Local Right Vector for the WorldNut
        /// </summary>
        /// <returns>Local Right Vector</returns>
        Vec3 GetLocalRight();

        /// <summary>
        /// Returns the Local Up Vector for the WorldNut
        /// </summary>
        /// <returns>Local Up Vector</returns>
        Vec3 GetLocalUp();

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
        /// Rotates by WorldNut by the super local X Axis, it's local right vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateSuperLocalX(float rad);

        /// <summary>
        /// Rotates by WorldNut by the super local X Axis, it's local up vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateSuperLocalY(float rad);

        /// <summary>
        /// Rotates by WorldNut by the super local X Axis, it's local forwards vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateSuperLocalZ(float rad);

        /*/// <summary>
        /// Rotates by WorldNut by the relative global X Axis, it's parent's right vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateRelativeGlobalX(float rad);

        /// <summary>
        /// Rotates by WorldNut by the relative global Y Axis, it's parent's up vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateRelativeGlobalY(float rad);

        /// <summary>
        /// Rotates by WorldNut by the relative global Y Axis, it's parent's forwards vector.
        /// </summary>
        /// <param name="rad">Radians to rotate by</param>
        void RotateRelativeGlobalZ(float rad);*/

        /// <summary>
        /// Returns the Local World Matrix of the WorldNut in Scale then Rotation then Translation Format. 
        /// </summary>
        /// <returns></returns>
        Mat4 GetLocalSRTWorldMatrix();

        /// <summary>
        /// Returns the Global World Matrix of the WorldNut in Scale then Rotation then Translation Format. 
        /// </summary>
        /// <returns></returns>
        Mat4 GetGlobalSRTWorldMatrix();

        /// <summary>
        /// Returns the Parent's Global World Matrix of the WorldNut in Scale then Rotation then Translation Format. 
        /// </summary>
        /// <returns></returns>
        Mat4 GetParentGlobalSRTWorldMatrix();

        virtual ~WorldNut() override {}
    private:

        // Override new child added to update all children's global matrices;
        virtual void NewChildAdded(bool myChild) override;

        Vec3 position;
        Vec3 eulerRotation;
        Quat rotation;
        Vec3 scale;

        Vec3 forward;
        Vec3 up;
        Vec3 right;

        Vec3 localForward;
        Vec3 localUp;
        Vec3 localRight;

        /*Vec3 relativeGlobalForward;
        Vec3 relativeGlobalUp;
        Vec3 relativeGlobalRight;*/

        /// <summary>
        /// Recalculates the local directions: right, up and forwards. 
        /// </summary>
        void RecalculateLocalDirections();

        Mat4 SRTWorldMatrixParent;
        Mat4 SRTWorldMatrixGlobal;
        Mat4 SRTWorldMatrixLocal;


        /// <summary>
        /// Recalculates all world matrices.
        /// </summary>
        void RecalculateWorldMatrices();

        /// <summary>
        /// Updates the transforms of this nut and all children
        /// </summary>
        /// <param name="nut">The nut to update transforms on</param>
        /// <param name="WorldMatrixSoFar">The nut's parent World Matrix</param>
        void UpdateTransforms(Nut* nut, Mat4 WorldMatrixSoFar);
    };
}