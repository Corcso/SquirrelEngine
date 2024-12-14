#include "PCH.h"
#include "PhysicsNut.h"
#include "Services.h"
namespace SQ {
    UniquePoolPtr<Nut> SQ::PhysicsNut::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
    {
        // Cast deserializeInto to our type, call it toWorkOn
        WorldNut* toWorkOn = dynamic_cast<WorldNut*>(deserializeInto);
        // If toWorkOn is nullptr, make a new nut of our type. 
        UniquePoolPtr<Nut> owner;
        if (deserializeInto == nullptr) {
            UniquePoolPtr<PhysicsNut> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<PhysicsNut>();
            toWorkOn = instance.get();
            owner = instance.StaticUniquePoolPtrCast<Nut>();
            deserializeInto = owner.get();
        }
        // Call parent deserialise, passing in our toWorkOn.
        Nut::Deserialize(toWorkOn, serializedData);

        // Perform deserialization on our data. 

        // Return toWorkOn
        return owner;
    }
}