#pragma once
#include "WorldNut.h"
#include "CollisionShape.h"

#include <set>
namespace SQ {
    /// <summary>
    /// Physics nut, a physically simulated nut which supports collision detection. 
    /// </summary>
    class PhysicsNut :
        public WorldNut
    {
    public:
        PhysicsNut() { isStatic = false; }
        /// <summary>
        /// Constructor for setting static or dynamic as its 1 time set. 
        /// </summary>
        /// <param name="isStatic">True if static, false if dynamic.</param>
        PhysicsNut(bool isStatic);

        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        // Make sure to call base class ready update and late update
        virtual void Ready() override;
        virtual void Update() override { WorldNut::Update(); }
        virtual void LateUpdate() override { WorldNut::LateUpdate(); }

        /// <summary>
        /// Triggers on collision enter, before update. 
        /// </summary>
        /// <param name="other">Nut collided with</param>
        virtual void OnCollisionStart(PhysicsNut* other) {}

        /// <summary>
        /// Triggers on collision exit, before update, after collision enter. 
        /// </summary>
        /// <param name="other">Nut exited from</param>
        virtual void OnCollisionEnd(PhysicsNut* other) {}

        /// <summary>
        /// Set the collision shape of this nut
        /// </summary>
        /// <param name="shape">Collision Shape Resource</param>
        void SetShape(std::shared_ptr<CollisionShape> shape);

        /// <summary>
        /// Get the collision shape resource for this nut
        /// </summary>
        /// <returns>Collision Shape Resource</returns>
        std::shared_ptr<CollisionShape> GetShape();

        /// <summary>
        /// Set elasticity (bounciness), 0 is 100% energy absorbed, 1 is 100% energy reflected. 
        /// </summary>
        /// <param name="elasticity">Elasticity</param>
        void SetElasticity(float elasticity);

        /// <summary>
        /// Set density, this determines the weight of the object. 
        /// </summary>
        /// <param name="density">Density</param>
        void SetDensity(float density);

        /// <summary>
        /// Get elasticity
        /// </summary>
        /// <returns>Elasticity</returns>
        float GetElasticity();
        /// <summary>
        /// Get density
        /// </summary>
        /// <returns>Density</returns>
        float GetDensity();

        /// <summary>
        /// <para>Sets the nut to be static or dynamic</para>
        /// <para>Note: Must be done before Ready() is called (Added to main scene tree). </para>
        /// </summary>
        /// <returns>True if static, false if dynamic</returns>
        void SetStatic(bool isStatic);

        /// <summary>
        /// Get if the object is static or dynamic
        /// </summary>
        /// <returns>True if static, false if dynamic</returns>
        bool IsStatic();

        /// <summary>
        /// Set which contains pointers to all currently colliding nuts. 
        /// </summary>
        std::set<PhysicsNut*> currentlyColliding;

        // force, torque, impulse and velocity 

        /// <summary>
        /// Adds force at the center of mass. 
        /// </summary>
        /// <param name="force">Force</param>
        void AddForce(Vec3 force);

        /// <summary>
        /// Applies an impulse at the center of mass. 
        /// </summary>
        /// <param name="impulse">Impulse</param>
        void AddImpulse(Vec3 impulse);

        /// <summary>
        /// Applies an angular impulse
        /// </summary>
        /// <param name="impulse">Impulse</param>
        void AddAngularImpulse(Vec3 impulse);

        /// <summary>
        /// Adds torque to the nut. 
        /// </summary>
        /// <param name="torque">Torque</param>
        void AddTorque(Vec3 torque);

        /// <summary>
        /// Sets linear velocity.
        /// </summary>
        /// <param name="velocity">Velocity</param>
        void SetLinearVelocity(Vec3 velocity);

        /// <summary>
        /// Sets angular velocity 
        /// </summary>
        /// <param name="velocity">Velocity</param>
        void SetAngularVelociry(Vec3 velocity);

        /// <summary>
        /// Destructor, removes the nut from the physics world. 
        /// </summary>
        ~PhysicsNut();

    private:
        float elasticity; 
        float density; 
        bool isStatic;

        std::shared_ptr<CollisionShape> shape;
    };

}