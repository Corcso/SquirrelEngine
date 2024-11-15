#include "PCH.h"
#ifdef WINDOWS

#include "InputWindows.h"
#include "Services.h"

// Temp 
#include <iostream>
namespace SQ {

    Input::Key virtualKeyCodeLookupTable[256] = {
       Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::BACKSPACE,
Input::Key::TAB,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::ENTER,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::SHIFT,
Input::Key::CONTROL,
Input::Key::ALT,
Input::Key::PAUSE,
Input::Key::CAPSLOCK,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::ESCAPE,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::SPACE,
Input::Key::PAGE_UP,
Input::Key::PAGE_DOWN,
Input::Key::HOME,
Input::Key::END,
Input::Key::LEFT_ARROW,
Input::Key::UP_ARROW,
Input::Key::RIGHT_ARROW,
Input::Key::DOWN_ARROW,
Input::Key::INVALID_KEY,
Input::Key::PRINT,
Input::Key::INVALID_KEY,
Input::Key::PRINT,
Input::Key::INSERT,
Input::Key::DEL,
Input::Key::INVALID_KEY,
Input::Key::ZERO,
Input::Key::ONE,
Input::Key::TWO,
Input::Key::THREE,
Input::Key::FOUR,
Input::Key::FIVE,
Input::Key::SIX,
Input::Key::SEVEN,
Input::Key::EIGHT,
Input::Key::NINE,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::A,
Input::Key::B,
Input::Key::C,
Input::Key::D,
Input::Key::E,
Input::Key::F,
Input::Key::G,
Input::Key::H,
Input::Key::I,
Input::Key::J,
Input::Key::K,
Input::Key::L,
Input::Key::M,
Input::Key::N,
Input::Key::O,
Input::Key::P,
Input::Key::Q,
Input::Key::R,
Input::Key::S,
Input::Key::T,
Input::Key::U,
Input::Key::V,
Input::Key::W,
Input::Key::X,
Input::Key::Y,
Input::Key::Z,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY

    };

    //InputWindows::InputWindows()
    //{
    //    // Setup Virtual Key Code lookup to Input::Key
    //    // Clear all values
    //    for (int c = 0; c < 256; ++c) {
    //        virtualKeyCodeLookupTable[c] = Key::INVALID_KEY;
    //    }
    //    // 0x08 is Backspace
    //    virtualKeyCodeLookupTable[0x08] = Key::BACKSPACE;
    //    // 0x09 is Tab
    //    virtualKeyCodeLookupTable[0x08] = Key::TAB;
    //    // TODO add L and R support
    //    virtualKeyCodeLookupTable[0x10] = Key::SHIFT;   // 0x10 is Shift
    //    virtualKeyCodeLookupTable[0x11] = Key::CONTROL; // 0x11 is Control
    //    virtualKeyCodeLookupTable[0x12] = Key::ALT;     // 0x12 is Alt
    //    virtualKeyCodeLookupTable[0x13] = Key::PAUSE;   // 0x13 is Pause
    //    virtualKeyCodeLookupTable[0x14] = Key::CAPSLOCK;// 0x14 is Capslock
    //    virtualKeyCodeLookupTable[0x1B] = Key::ESCAPE;  // 0x1B is Escape
    //    virtualKeyCodeLookupTable[0x20] = Key::SPACE;  // 0x20 is Escape


    //}

    LRESULT InputWindows::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT paintStruct;
        HDC hDC;
        switch (message)
        {
        case WM_KEYUP:
        {
            //int scanCode = (lParam & 0x00FF0000) >> 16;
            //std::cout << "UP: " << scanCode << "\n";
            Services::GetInput()->SetKeyState(virtualKeyCodeLookupTable[wParam], InputState::UP);
            
        }
        break;
        case WM_KEYDOWN:
        {
            //int scanCode = (lParam & 0x00FF0000) >> 16;
            //std::cout << "DOWN: " << scanCode << "\n";
            Services::GetInput()->SetKeyState(virtualKeyCodeLookupTable[wParam], InputState::DOWN);
        }
        break;
        case WM_PAINT:
        {
            hDC = BeginPaint(hwnd, &paintStruct);
            EndPaint(hwnd, &paintStruct);
        }
        break;
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }

    void InputWindows::Update()
    {
    }
}
#endif //WINDOWS