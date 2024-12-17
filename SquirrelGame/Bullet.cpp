#include "Bullet.h"

SQ::UniquePoolPtr<SQ::Nut> Bullet::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    Bullet* toWorkOn = dynamic_cast<Bullet*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<Bullet> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Bullet>();
        // Set to work on to the instance
        toWorkOn = instance.get();
        // Transfer ownership into owner and static cast to nut base class
        owner = instance.StaticUniquePoolPtrCast<Nut>();
    }
    // REQUIRED PARENT DESERIALIZATION, CALL PARENT CLASS
    // Call parent deserialise, passing in our toWorkOn.
    PhysicsNut::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 

    // YOUR DESERIALIZATION HERE, USE TOWORKON

    // Return ownership of the new nut
    return owner;
}

void Bullet::Ready()
{
    // Call base ready (required)
    PhysicsNut::Ready();

    SetLinearVelocity(GetForward() * 100);
}

void Bullet::Update()
{
    // Call base update (required)
    PhysicsNut::Update();
}

void Bullet::LateUpdate()
{
    // Call base late update (required)
    PhysicsNut::LateUpdate();
}

void Bullet::OnCollisionStart(PhysicsNut* other)
{
    // If we have collided with something which isnt the player. Destroy.
    if (other->name != "Main Player") QueueDestroy();
}
