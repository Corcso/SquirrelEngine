#include "AirlockSceneLoading.h"
#include "Player.h"

SQ::UniquePoolPtr<SQ::Nut> AirlockSceneLoading::Deserialize(Nut* deserializeInto, nlohmann::json serializedData)
{
    // PERFORM PRE DESERIALIZE WORK
    AirlockSceneLoading* toWorkOn = dynamic_cast<AirlockSceneLoading*>(deserializeInto);
    UniquePoolPtr<Nut> owner;
    if (deserializeInto == nullptr) {
        UniquePoolPtr<AirlockSceneLoading> instance = Services::GetPoolAllocationService()->MakeUniquePoolPtr<AirlockSceneLoading>();
        toWorkOn = instance.get();
        owner = instance.StaticUniquePoolPtrCast<Nut>();
        deserializeInto = owner.get();
    }
    // CALL PARENT DESERIALIZE FIRST
    Nut::Deserialize(toWorkOn, serializedData);

    // Perform deserialization on our data. 


    // Return ownership of the new nut
    return owner;
}

void AirlockSceneLoading::Ready()
{
    // Call base ready (required)
    Nut::Ready();

    packedScene = GetResourceManager()->Retrieve<ShelledNut>("./Resources/secondLevel.nut");
}

void AirlockSceneLoading::Update()
{
    // Call base update (required)
    Nut::Update();

    Player* player = GetTree()->GetRootNut()->GetNut<Player>("Scene/Main Player");
    if (player && promise.get() == nullptr && LenSqrV3(player->GetGlobalPosition() - V3(0, 0, 20)) <= 25) {
        promise = packedScene->InstantiateMultithread();
    }

    if (!loaded && promise.get() != nullptr && promise->complete) {
        loaded = true;
        UniquePoolPtr<Nut> newLevel = std::move(promise->result);
        Nut* observer = newLevel.get();
        observer->SetParent(GetTree()->GetRootNut(), std::move(newLevel));
    }
}

void AirlockSceneLoading::LateUpdate()
{
    // Call base late update (required)
    Nut::LateUpdate();
}