#pragma once
#include "SquirrelEngine.h"
class Spinner :
    public SQ::MeshNut
{
public:
    static Nut* Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    void Update() override;
};

