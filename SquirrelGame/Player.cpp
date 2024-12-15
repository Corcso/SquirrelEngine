#include "Player.h"

SQ::UniquePoolPtr<SQ::Nut> Player::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // PERFORM PRE DESERIALIZE WORK
    Player* toWorkOn = dynamic_cast<Player*>(deserializeInto);
    UniquePoolPtr<Nut> owner;
    if (deserializeInto == nullptr) {
        UniquePoolPtr<Player> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Player>();
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

void Player::Ready()
{
    // Call base ready (required)
    PhysicsNut::Ready();
}

void Player::Update()
{
    // Call base update (required)
    PhysicsNut::Update();

    if (SQ::Services::GetInput()->IsKeyPressed('q')) SQ::Services::GetInput()->LockMouse();
    else if (SQ::Services::GetInput()->IsKeyReleased('e')) SQ::Services::GetInput()->UnlockMouse();

    if (GetInput()->IsKeyDown('A')) SetLinearVelocity(-GetRight() * 4);
    if (GetInput()->IsKeyDown('D')) SetLinearVelocity(GetRight() * 4);
    if (GetInput()->IsKeyDown('W')) SetLinearVelocity(GetForward() * 4);
    if (GetInput()->IsKeyDown('S')) SetLinearVelocity(-GetForward() * 4);

    if (SQ::Services::GetInput()->IsMouseLocked()) {
        RotateGlobalY(SQ::Services::GetInput()->GetMouseMovement().X * -0.01);
        GetNthChild<CameraNut>(0)->RotateSuperLocalX(SQ::Services::GetInput()->GetMouseMovement().Y * -0.01);
    }
}

void Player::LateUpdate()
{
    // Call base late update (required)
    PhysicsNut::LateUpdate();


}
