#include "Player.h"
#include "Bullet.h"
SQ::UniquePoolPtr<SQ::Nut> Player::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    Player* toWorkOn = dynamic_cast<Player*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<Player> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<Player>();
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

void Player::Ready()
{
    // Call base ready (required)
    PhysicsNut::Ready();

    // Example of preloading resources if we are using inthread instantiation. This removes the need to load them which takes time. 
    // Useful for things like bullets where they are being created and destroyed often, but we know as long as there is a player there could be bullets. 
    // You can either use the preload to set the mesh or retrieve it again, both dont load the bullet but using the preload is slightly faster as the resource manager doesn't need called. 
    bulletMeshPreload = GetResourceManager()->Retrieve<Mesh>("./Resources/bullet.obj");
}

void Player::Update()
{
    // Call base update (required)
    PhysicsNut::Update();

    // Press Q to lock the mouse, Press E to unlock it. 
    if (SQ::Services::GetInput()->IsKeyPressed('q')) SQ::Services::GetInput()->LockMouse();
    else if (SQ::Services::GetInput()->IsKeyReleased('e')) SQ::Services::GetInput()->UnlockMouse();
    
    //Only move if colliding with the floor
    bool onFloor = false;
    for (std::set<PhysicsNut*>::iterator it = currentlyColliding.begin(); it != currentlyColliding.end(); ++it) {
        if ((*it)->name == "Floor") onFloor = true;
    }
    if (onFloor) {
        // Take in lateral movement with wasd
        Vec3 movementDesire = V3(0, 0, 0);
        if (GetInput()->IsKeyDown('A')) movementDesire += -GetRight();
        else if (GetInput()->IsKeyDown('D')) movementDesire += GetRight();
        if (GetInput()->IsKeyDown('W')) movementDesire += GetForward();
        else if (GetInput()->IsKeyDown('S')) movementDesire += -GetForward();
        // Use desire to move the player, normalize if desire length is more than 0, otherwise don't as can't normalize a 0 length vector
        if (LenSqrV3(movementDesire) > 0) movementDesire = NormV3(movementDesire);
        SetLinearVelocity(movementDesire * 4);
        // Remove all angular velocity
        SetAngularVelociry(V3(0, 0, 0));

        // Process a jump
        if (GetInput()->IsKeyPressed(Input::Key::SPACE)) AddImpulse(V3(0, 15, 0));
    }
    // If the mouse is locked, take in mouse input
    if (SQ::Services::GetInput()->IsMouseLocked()) {
        RotateGlobalY(SQ::Services::GetInput()->GetMouseMovement().X * -0.01);
        GetNthChild<CameraNut>(0)->RotateSuperLocalX(SQ::Services::GetInput()->GetMouseMovement().Y * -0.01);
    }
    
    // Shoot a bullet
    if (GetInput()->IsMousePressed(Input::MouseButton::LEFT)) {
        // Manually create a nut and child rather than loading from file. 

        // The pool service must be used to create the nut in memory
        UniquePoolPtr<Bullet> newBullet = GetPoolAllocationService()->MakeUniquePoolPtr<Bullet>();
        // Create the child, mesh nut
        UniquePoolPtr<MeshNut> childMesh = GetPoolAllocationService()->MakeUniquePoolPtr<MeshNut>();
        // Setup child properties
        childMesh->name = "Bullet Mesh";
        childMesh->SetMesh(bulletMeshPreload);
        childMesh->SetMaterial(GetResourceManager()->Retrieve<Material>("./Resources/bullet.mat"));
        childMesh->SetEulerAngles(V3(0, 3.14f, 0));
        // Get observer to child (as we are relinquishing ownership) and use that to reparent the child to the bullet. 
        MeshNut* observerOfMesh = childMesh.get();
        observerOfMesh->SetParent(newBullet.get(), childMesh.StaticUniquePoolPtrCast<Nut>());
        // Setup the bullet
        newBullet->name = "Bullet";
        newBullet->SetStatic(false); // Not explicitally needed as dynamic is the default but here for example. Must be called before ready is called as this cant be changed once it begins physics simulation. 
        newBullet->SetElasticity(0);
        newBullet->SetDensity(1);
        newBullet->SetShape(GetResourceManager()->Retrieve<CollisionShape>("./Resources/bullet.shape"));
        newBullet->SetPosition(GetPosition());
        newBullet->SetRotation(GetRotation());
        // Get observer to bullet and reparent to scene. (Ready will now be called and bulled will be registered with physics system)
        Bullet* observer = newBullet.get();
        observer->SetParent(GetTree()->GetRootNut(), newBullet.StaticUniquePoolPtrCast<Nut>());
    }

    // Set all things using the players collider shape (enemies and player) to be spheres instead
    if (GetInput()->IsKeyPressed('K')) {
        GetShape()->SetSphereRadius(1);
        GetShape()->SetType(CollisionShape::Type::SPHERE);
    }

    // Here for debugging purposes. Press G to output Pool Allocator Capacity. 
    if (SQ::GetInput()->IsKeyPressed('G')) {
        std::cout << SQ::GetPoolAllocationService()->GetDebugPoolUsageStatement();
    }
}

void Player::LateUpdate()
{
    // Call base late update (required)
    PhysicsNut::LateUpdate();
}
