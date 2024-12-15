#include "Player.h"
#include "Bullet.h"
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

    packedBullet = GetResourceManager()->Retrieve<ShelledNut>("./Resources/bullet.nut");
}

void Player::Update()
{
    // Call base update (required)
    PhysicsNut::Update();

    // Press Q to lock the mouse, Press E to unlock it. 
    if (SQ::Services::GetInput()->IsKeyPressed('q')) SQ::Services::GetInput()->LockMouse();
    else if (SQ::Services::GetInput()->IsKeyReleased('e')) SQ::Services::GetInput()->UnlockMouse();

    // Take in lateral movement with wasd
    if (GetInput()->IsKeyDown('A')) SetLinearVelocity(-GetRight() * 4);
    else if (GetInput()->IsKeyDown('D')) SetLinearVelocity(GetRight() * 4);
    if (GetInput()->IsKeyDown('W')) SetLinearVelocity(GetForward() * 4);
    else if (GetInput()->IsKeyDown('S')) SetLinearVelocity(-GetForward() * 4);

    // If the mouse is locked, take in mouse input
    if (SQ::Services::GetInput()->IsMouseLocked()) {
        RotateGlobalY(SQ::Services::GetInput()->GetMouseMovement().X * -0.01);
        GetNthChild<CameraNut>(0)->RotateSuperLocalX(SQ::Services::GetInput()->GetMouseMovement().Y * -0.01);
    }
    
    // Shoot a bullet
    if (GetInput()->IsMouseReleased(Input::MouseButton::LEFT)) {
        UniquePoolPtr<Bullet> newBullet = packedBullet->Instantiate().DynamicUniquePoolPtrCast<Bullet>();
        newBullet->SetPosition(GetPosition());
        newBullet->SetRotation(GetRotation());
        Bullet* observer = newBullet.get();
        observer->SetParent(GetTree()->GetRootNut(), newBullet.StaticUniquePoolPtrCast<Nut>());
    }

    // Set all things using the players collider shape (enemies and player) to be spheres instead
    if (GetInput()->IsKeyPressed('K')) {
        GetShape()->SetSphereRadius(1);
        GetShape()->SetType(CollisionShape::Type::SPHERE);
    }
}

void Player::LateUpdate()
{
    // Call base late update (required)
    PhysicsNut::LateUpdate();
}
