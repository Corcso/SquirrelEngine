#pragma once
#include "WorldNut.h"
#include "CollisionShape.h"

#include <set>
namespace SQ {
    class PhysicsNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        virtual void OnCollisionStart(PhysicsNut* other) {}
        virtual void OnCollisionEnd(PhysicsNut* other) {}

        void SetShape(std::shared_ptr<CollisionShape> mesh);
        std::shared_ptr<CollisionShape> GetShape();

        float GetElasticity();
        float GetDensity();
        bool IsStatic();

        std::set<PhysicsNut*> currentlyColliding;

    private:
        float elasticity; 
        float density; 
        bool isStatic;

        std::shared_ptr<CollisionShape> shape;
    };

}