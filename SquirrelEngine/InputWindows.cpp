#include "PCH.h"
#ifdef WINDOWS

#include "InputWindows.h"
#include "Services.h"

// Temp 
#include <iostream>
namespace SQ {

    // Setup the virtual key code lookup table
    // This array converts windows virtual key codes to Squirrel Engine Key Enums
    // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    Input::Key InputWindows::virtualKeyCodeLookupTable[256] = {
Input::Key::INVALID_KEY,
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
Input::Key::INVALID_KEY,
Input::Key::INVALID_KEY


    };

    InputWindows::InputWindows()
    {
        // Set mouse position to where it is, to prevent mouse jolting in the first frame
        LPPOINT mousePos = new POINT;
        GetCursorPos(mousePos);
        mousePositionThisFrame = V2(mousePos->x, mousePos->y);
        delete mousePos;
    }

    LRESULT InputWindows::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT paintStruct;
        HDC hDC;
        switch (message)
        {
        // Process key up and down events
        case WM_KEYUP:
        {
            Services::GetInput()->SetKeyState(virtualKeyCodeLookupTable[wParam], InputState::UP);
            
        }
        break;
        case WM_KEYDOWN:
        {
            Services::GetInput()->SetKeyState(virtualKeyCodeLookupTable[wParam], InputState::DOWN);
        }
        break;
        // Process mouse button events
        case WM_LBUTTONDOWN:
        {
            Services::GetInput()->SetMouseState(Input::MouseButton::LEFT, InputState::DOWN);
        }
        break;
        case WM_LBUTTONUP:
        {
            Services::GetInput()->SetMouseState(Input::MouseButton::LEFT, InputState::UP);
        }
        case WM_RBUTTONDOWN:
        {
            Services::GetInput()->SetMouseState(Input::MouseButton::RIGHT, InputState::DOWN);
        }
        break;
        case WM_RBUTTONUP:
        {
            Services::GetInput()->SetMouseState(Input::MouseButton::RIGHT, InputState::UP);
        }
        break;

        // Use the mouse move event if the mouse isnt locked otherwise use the WM_INPUT raw input event (Microsoft, 2023)
        // https://learn.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
        case WM_MOUSEMOVE:
        {
            if (Services::GetInput()->IsMouseLocked()) break;
            int xCoord =  0x0000FFFF & lParam;
            int yCoord = (0xFFFF0000 & lParam) >> 16;
            Services::GetInput()->SetMousePosition(V2(xCoord, yCoord));
            Services::GetInput()->SetMouseMovement(V2(xCoord, yCoord) - Services::GetInput()->GetMousePositionLastFrame());

        }
        break;
        case WM_INPUT:
        {
            if (!Services::GetInput()->IsMouseLocked()) break;
            UINT dwSize = sizeof(RAWINPUT);
            static BYTE lpb[sizeof(RAWINPUT)];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                int xPosRelative = raw->data.mouse.lLastX;
                int yPosRelative = raw->data.mouse.lLastY;
                Services::GetInput()->SetMouseMovement(V2(xPosRelative, yPosRelative));
            }
            break;
        }
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
        Input::Update();

        // If we are locked, set mouse movement to 0 at end of the frame
        if (isMouseLocked) mouseMovement = V2(0, 0);
    }

    void InputWindows::ProcessInput()
    {
        MSG msg = { 0 };
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (isMouseLocked) {
            SetCursorPos((Services::GetGraphics()->GetWindowLocation() + (Services::GetGraphics()->GetRenderWindowSize() / 2.0f)).X, (Services::GetGraphics()->GetWindowLocation() + (Services::GetGraphics()->GetRenderWindowSize() / 2.0f)).Y);
        }
    }

    void InputWindows::LockMouse()
    {
        ShowCursor(false);
        isMouseLocked = true;
    }

    void InputWindows::UnlockMouse()
    {
        ShowCursor(true);
        isMouseLocked = false;
    }
}
#endif //WINDOWS