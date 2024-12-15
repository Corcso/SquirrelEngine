#pragma once
#include "SquirrelEngine.h"

using namespace SQ;
class Bullet :
    public PhysicsNut
{
public:
    static SQ::UniquePoolPtr<SQ::Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    virtual void Ready() override;
    virtual void Update() override;
    virtual void LateUpdate() override;

    virtual void OnCollisionStart(PhysicsNut* other) override;
};

