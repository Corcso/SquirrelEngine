#include "Bullet.h"

SQ::UniquePoolPtr<SQ::Nut> Bullet::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // PERFORM PRE DESERIALIZE WORK
    Bullet* toWorkOn = dynamic_cast<Bullet*>(deserializeInto);
    UniquePoolPtr<Nut> owner;
    if (deserializeInto == nullptr) {
        UniquePoolPtr<Bullet> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Bullet>();
        toWorkOn = instance.get();
        owner = instance.StaticUniquePoolPtrCast<Nut>();
        deserializeInto = owner.get();
    }
    // CALL PARENT DESERIALIZE FIRST
    PhysicsNut::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 


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
    if (other->name != "Main Player") QueueDestroy();
}
