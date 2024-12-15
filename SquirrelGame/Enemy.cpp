#include "Enemy.h"
#include "Bullet.h"

SQ::UniquePoolPtr<SQ::Nut> Enemy::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // PERFORM PRE DESERIALIZE WORK
    Enemy* toWorkOn = dynamic_cast<Enemy*>(deserializeInto);
    UniquePoolPtr<Nut> owner;
    if (deserializeInto == nullptr) {
        UniquePoolPtr<Enemy> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Enemy>();
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

void Enemy::Ready()
{
    // Call base ready (required)
    PhysicsNut::Ready();
}

void Enemy::Update()
{
    // Call base update (required)
    PhysicsNut::Update();

    Vec3 toPlayer = -NormV3(GetGlobalPosition() - GetTree()->GetRootNut()->GetNut<WorldNut>("Scene/Main Player")->GetGlobalPosition());

    SetLinearVelocity(toPlayer * 2);
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
