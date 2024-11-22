#pragma once
#include "CameraNut.h"

using namespace SQ;
class FPVCamera :
    public CameraNut
{
public:
    void Update() override;
};

