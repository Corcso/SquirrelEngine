#pragma once
#include "Resource.h"
#include "SQMath.h"
namespace SQ {
    class CollisionShape :
        public Resource
    {
    public:
        enum class Type {
            BOX,
            SPHERE,
            CAPSULE
        };

        /// <summary>
        /// Loads a collision shape and returns a pointer to it. 
        /// </summary>
        /// <param name="path">Path of the collision shape .shape file</param>
        /// <returns>A pointer to the new collision shape</returns>
        static CollisionShape* Load(std::string path);

        void SetType(Type type);
        void SetBoxHalfDimentions(Vec3 halfDimentions);
        void SetCapsuleRadius(float radius);
        void SetCapsuleHalfHeight(float halfHeight);
        void SetSphereRadius(float radius);

        Type GetType();
        Vec3 GetBoxHalfDimentions();
        float GetCapsuleRadius();
        float GetCapsuleHalfHeight();
        float GetSphereRadius();

        virtual void Update() {}
    public:
        Type type;
        Vec3 boxHalfDimentions;
        float capsuleRadius;
        float capsuleHalfHeight;
        float sphereRadius;
    };
}