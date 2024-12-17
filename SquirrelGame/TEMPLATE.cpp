#include "TEMPLATE.h"

UniquePoolPtr<Nut> CLASSNAME::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    CLASSNAME* toWorkOn = dynamic_cast<CLASSNAME*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<CLASSNAME> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<CLASSNAME>();
        // Set to work on to the instance
        toWorkOn = instance.get();
        // Transfer ownership into owner and static cast to nut base class
        owner = instance.StaticUniquePoolPtrCast<Nut>();
    }
    // REQUIRED PARENT DESERIALIZATION, CALL PARENT CLASS
    // Call parent deserialise, passing in our toWorkOn.
    PARENTNAME::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 

    // YOUR DESERIALIZATION HERE, USE TOWORKON

    // Return ownership of the new nut
    return owner;
}

void CLASSNAME::Ready()
{
    // Call base ready (required)
    PARENTNAME::Ready();
}

void CLASSNAME::Update()
{
    // Call base update (required)
    PARENTNAME::Update();
}

void CLASSNAME::LateUpdate()
{
    // Call base late update (required)
    PARENTNAME::LateUpdate();
}