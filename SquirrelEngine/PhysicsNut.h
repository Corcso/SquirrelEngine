#pragma once
#include "WorldNut.h"

#include <set>
namespace SQ {
    class PhysicsNut :
        public WorldNut
    {
    public:
        static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

        virtual void OnCollisionStart(PhysicsNut* other) {}
        virtual void OnCollisionEnd(PhysicsNut* other) {}

        std::set<PhysicsNut*> currentlyColliding;

    private:
    };

}