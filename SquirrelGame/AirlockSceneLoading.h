#pragma once
#include "SquirrelEngine.h"

using namespace SQ;
class AirlockSceneLoading :
    public Nut
{
public:
    static SQ::UniquePoolPtr<SQ::Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    virtual void Ready() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
private:
    std::shared_ptr<ShelledNut> packedScene;
    std::shared_ptr<ShelledNut::InstantiatePromise> promise;
    bool loaded;
};

