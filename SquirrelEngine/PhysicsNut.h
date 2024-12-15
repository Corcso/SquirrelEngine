#pragma once
#include "WorldNut.h"
#include "CollisionShape.h"

#include <set>
namespace SQ {
    class PhysicsNut :
        public WorldNut
    {
    public:
        PhysicsNut() { isStatic = false; }
        PhysicsNut(bool isStatic);

        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        // Make sure to call base class ready update and late update
        virtual void Ready() override;
        virtual void Update() override { WorldNut::Update(); }
        virtual void LateUpdate() override { WorldNut::LateUpdate(); }

        virtual void OnCollisionStart(PhysicsNut* other) {}
        virtual void OnCollisionEnd(PhysicsNut* other) {}

        void SetShape(std::shared_ptr<CollisionShape> mesh);
        std::shared_ptr<CollisionShape> GetShape();

        void SetElasticity(float elasticity);
        void SetDensity(float density);
        float GetElasticity();
        float GetDensity();
        bool IsStatic();

        std::set<PhysicsNut*> currentlyColliding;

        // force, torque, impulse and velocity 
        void AddForce(Vec3 force);
        void AddImpulse(Vec3 impulse);
        void AddAngularImpulse(Vec3 impulse);
        void AddTorque(Vec3 torque);
        void SetLinearVelocity(Vec3 velocity);
        void SetAngularVelociry(Vec3 velocity);

        ~PhysicsNut();

    private:
        float elasticity; 
        float density; 
        bool isStatic;

        std::shared_ptr<CollisionShape> shape;
    };

}