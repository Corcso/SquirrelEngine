#pragma once
#include "SquirrelEngine.h"

using namespace SQ;
/// <summary>
/// Example free floating first person camera nut
/// </summary>
class FPVCamera :
    public CameraNut
{
public:
    static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    virtual void Ready() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};

