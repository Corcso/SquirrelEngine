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

        /// <summary>
        /// Performs special input actions on top of default ones, for windows. 
        /// </summary>
        virtual void Update();

        /// <summary>
        /// <para>Processes the input for windows builds. </para>
        /// <para>Due to the nature of this, a HWND reference is not needed</para>
        /// </summary>
        virtual void ProcessInput() final;

        virtual void LockMouse() final;
        virtual void UnlockMouse() final;
    private:
        /// <summary>
        /// A lookup table converting windows virtual key codes into Squirrel Engine Keys
        /// </summary>
        static Key virtualKeyCodeLookupTable[256];
    };
}
#endif //WINDOWS