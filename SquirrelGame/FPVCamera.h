#pragma once
#include "SquirrelEngine.h"

using namespace SQ;
class FPVCamera :
    public CameraNut
{
public:
    static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    void Update() override;
};

