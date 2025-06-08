#pragma once
#include "CameraNut.h"

namespace SQ {
    class EditorCameraNut :
        public CameraNut
    {
    public:
        virtual void Ready() override;
        virtual void Update() override;
        virtual void LateUpdate() override;
    };
}

