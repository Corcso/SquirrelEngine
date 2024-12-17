#include "Enemy.h"
#include "Bullet.h"

SQ::UniquePoolPtr<SQ::Nut> Enemy::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    Enemy* toWorkOn = dynamic_cast<Enemy*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<Enemy> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Enemy>();
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

    // Example custom deserialization
    if (!serializedData["speed"].is_null()) toWorkOn->speed = serializedData["speed"];

    // Return ownership of the new nut
    return owner;
}

void Enemy::Ready()
{
    // Call base ready (required)
    PhysicsNut::Ready();
}

void Enemy::Update()
{
    // Call base update (required)
    PhysicsNut::Update();

    // Get the distance toward the player, checking the player exists. 
    if (GetTree()->GetRootNut()->GetNut<WorldNut>("Scene/Main Player") != nullptr) {
        Vec3 toPlayer = -NormV3(GetGlobalPosition() - GetTree()->GetRootNut()->GetNut<WorldNut>("Scene/Main Player")->GetGlobalPosition());
        // Set velocity toward the player
        SetLinearVelocity(toPlayer * speed);
        // Stop rotations
        SetEulerAngles(V3(0, 0, 0));
    }
}

void Enemy::LateUpdate()
{
    // Call base late update (required)
    PhysicsNut::LateUpdate();


}

void Enemy::OnCollisionStart(PhysicsNut* other)
{
    if (dynamic_cast<Bullet*>(other)) QueueDestroy();
}
