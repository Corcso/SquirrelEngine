#include "PCH.h"
#ifdef WINDOWS

#include "InputWindows.h"
#include "Services.h"

// Temp 
#include <iostream>
namespace SQ {

    LRESULT InputWindows::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT paintStruct;
        HDC hDC;
        switch (message)
        {
        case WM_KEYUP:
        {
            int scanCode = (lParam & 0x00FF0000) >> 16;
            std::cout << "UP: " << scanCode << "\n";
            Services::GetInput()->SetKeyState(scanCode, InputState::UP);
            
        }
        break;
        case WM_KEYDOWN:
        {
            int scanCode = (lParam & 0x00FF0000) >> 16;
            std::cout << "DOWN: " << scanCode << "\n";
            Services::GetInput()->SetKeyState(scanCode, InputState::DOWN);
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