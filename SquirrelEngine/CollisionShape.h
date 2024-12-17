#pragma once
#include "Resource.h"
#include "SQMath.h"
#include <set>
namespace SQ {

    // Forward declare PhysicsNut
    class PhysicsNut;

    /// <summary>
    /// Collision shape used by physics nuts. 
    /// </summary>
    class CollisionShape :
        public Resource
    {
    public:
        /// <summary>
        /// Types of shape available. 
        /// </summary>
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

        /// <summary>
        /// Set the type of shape
        /// </summary>
        /// <param name="type">Type</param>
        void SetType(Type type);
        /// <summary>
        /// Set the box half dimensions. Set this = scale of your mesh. 
        /// </summary>
        /// <param name="halfDimensions">Box Half Dimensions</param>
        void SetBoxHalfDimensions(Vec3 halfDimensions);

        /// <summary>
        /// Set capsule radius
        /// </summary>
        /// <param name="radius">Capsule radius</param>
        void SetCapsuleRadius(float radius);

        /// <summary>
        /// Set capsule half height, the height from center to edges of hemispheres. 
        /// </summary>
        /// <param name="halfHeight">Capsule half height</param>
        void SetCapsuleHalfHeight(float halfHeight);

        /// <summary>
        /// Set the sphere radius
        /// </summary>
        /// <param name="radius">Sphere radius</param>
        void SetSphereRadius(float radius);

        /// <summary>
        /// Returns the type of collision shape
        /// </summary>
        /// <returns>Type</returns>
        Type GetType();

        /// <summary>
        /// Returns the box half dimensions. 
        /// </summary>
        /// <returns>Box half dimensions</returns>
        Vec3 GetBoxHalfDimensions();

        /// <summary>
        /// Returns the capsule radius
        /// </summary>
        /// <returns>Capsule radius</returns>
        float GetCapsuleRadius();

        /// <summary>
        /// Returns the capsule half height
        /// </summary>
        /// <returns>Capsule half height</returns>
        float GetCapsuleHalfHeight();

        /// <summary>
        /// Returns the sphere radius
        /// </summary>
        /// <returns>Sphere radius</returns>
        float GetSphereRadius();

        /// <summary>
        /// Called when an update on the physics system is needed. 
        /// </summary>
        void Updated();

        /// <summary>
        /// Registers a nut against this shape, so all nuts with this shape can be easilly updated. 
        /// </summary>
        /// <param name="nut">Nut</param>
        void RegisterNut(PhysicsNut* nut);

        /// <summary>
        /// Deregisters a nut against this shape, stops nut from being updated when shape is updated. 
        /// </summary>
        /// <param name="nut"></param>
        void RemoveNut(PhysicsNut* nut);
    public:
        Type type;
        Vec3 boxHalfDimensions;
        float capsuleRadius;
        float capsuleHalfHeight;
        float sphereRadius;

        /// <summary>
        /// Set of nuts this shape is active on. 
        /// </summary>
        std::set<PhysicsNut*> activeOnTheseNuts;
    };
}