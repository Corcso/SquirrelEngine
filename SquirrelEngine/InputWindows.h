#pragma once

#ifdef WINDOWS

#include "Input.h"
#include <Windows.h>

namespace SQ {
    class InputWindows :
        public Input
    {
    public:
        InputWindows();

        // Windows event callback function
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
        virtual void Update();

        virtual void ProcessInput() final;

        virtual void LockMouse() final;
        virtual void UnlockMouse() final;
    private:
        static Key virtualKeyCodeLookupTable[256];
    };
}
#endif //WINDOWS