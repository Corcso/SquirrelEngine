#pragma once

#ifdef WINDOWS

#include "Input.h"
#include <Windows.h>

namespace SQ {
    class InputWindows :
        public Input
    {
    public:
        Key virtualKeyCodeLookupTable[256];
        // Windows event callback function (here for now should be moved over to input class
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

        virtual void Update() final;
    };
}
#endif //WINDOWS