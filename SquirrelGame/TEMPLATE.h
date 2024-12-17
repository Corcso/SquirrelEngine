#pragma once
#include "SquirrelEngine.h"
using namespace SQ;

class CLASSNAME :
    public PARENTNAME
{
public:
    static UniquePoolPtr<Nut> Deserialize(Nut* deserializeInto, nlohmann::json serializedData);

    virtual void Ready() override;
    virtual void Update() override;
    virtual void LateUpdate() override;
};

