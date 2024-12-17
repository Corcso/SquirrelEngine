#include "AirlockSceneLoading.h"
#include "Player.h"

SQ::UniquePoolPtr<SQ::Nut> AirlockSceneLoading::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // REQUIRED DESERIALIZATION CODE BLOCK, MAKE TOWORKON & INSTANCE TO YOUR NUT TYPE
    // Cast deserializeInto to our type, call it toWorkOn
    AirlockSceneLoading* toWorkOn = dynamic_cast<AirlockSceneLoading*>(deserializeInto);
    // If toWorkOn is nullptr, make a new nut of our type. 
    // We need to follow strict ownership with the pool ptr
    UniquePoolPtr<Nut> owner;
    if (toWorkOn == nullptr) {
        // Get the instance
        UniquePoolPtr<AirlockSceneLoading> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<AirlockSceneLoading>();
        // Set to work on to the instance
        toWorkOn = instance.get();
        // Transfer ownership into owner and static cast to nut base class
        owner = instance.StaticUniquePoolPtrCast<Nut>();
    }
    // REQUIRED PARENT DESERIALIZATION, CALL PARENT CLASS
    // Call parent deserialise, passing in our toWorkOn.
    Nut::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 

    // YOUR DESERIALIZATION HERE, USE TOWORKON

    // Return ownership of the new nut
    return owner;
}

void AirlockSceneLoading::Ready()
{
    // Call base ready (required)
    Nut::Ready();

    // Load our ShelledNut (note this does not load the scene)
    packedScene = GetResourceManager()->Retrieve<ShelledNut>("./Resources/secondLevel.nut");
}

void AirlockSceneLoading::Update()
{
    // Call base update (required)
    Nut::Update();

    // Check the player is inside the airlock area.
    Player* player = GetTree()->GetRootNut()->GetNut<Player>("Scene/Main Player");
    if (player && promise.get() == nullptr && LenSqrV3(player->GetGlobalPosition() - V3(0, 0, 20)) <= 25) {
        // If so start the scene loading on another thread
        promise = packedScene->InstantiateMultithread();
    }

    // If we haven't added the scene to the main tree
    // And we have a promise
    // And that promise shows that the scene has completed loading. 
    if (!loaded && promise.get() != nullptr && promise->complete) {
        // Set loaded to true
        loaded = true;
        // Get ownership of the new level from the nut.
        UniquePoolPtr<Nut> newLevel = std::move(promise->result);
        // Get an observer to the nut
        Nut* observer = newLevel.get();
        // Hand ownership over to the main tree and place the scene in the main tree. 
        observer->SetParent(GetTree()->GetRootNut(), std::move(newLevel));
    }
}

void AirlockSceneLoading::LateUpdate()
{
    // Call base late update (required)
    Nut::LateUpdate();
}