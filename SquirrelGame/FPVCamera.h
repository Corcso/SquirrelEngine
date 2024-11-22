#pragma once
#include "SquirrelEngine.h"

using namespace SQ;
class FPVCamera :
    public CameraNut
{
public:
    void Update() override;
};

