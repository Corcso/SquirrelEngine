#include "PCH.h"
#ifdef VULKAN

#include "GraphicsVulkan.h"

#include "InputWindows.h"

// For HD mouse movement (Microsoft, 2023)
#include <hidusage.h>

#include "SQUtility.h"

int SQ::GraphicsVulkan::Init(std::string title, int width, int height, Vec4 clearColor)
{
    // Begin Windows Window Setup

        // Define and register window class with OS
    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = &InputWindows::WndProc;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = WINDOW_CLASS_NAME;

    if (!RegisterClassEx(&windowClass)) {
        throw 2;
    }

    // Setup window

    // Client rect is the size of the renderable area (entire window)
    RECT clientRect = { 0, 0, width, height };
    // Window rect is the client rect asjusted for the top bar
    RECT windowRect = clientRect;
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    window = CreateWindowW(WINDOW_CLASS_NAME, NStringToWString(title).c_str(),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr, nullptr, GetModuleHandle(NULL), nullptr);

    if (!window) {
        throw 3;
    }

    ShowWindow(window, SW_NORMAL);
    UpdateWindow(window);

    //---------------------
    // Setup raw input for HD mouse movement (Microsoft, 2023)
    // https://learn.microsoft.com/en-us/windows/win32/dxtecharts/taking-advantage-of-high-dpi-mouse-movement
    #ifndef HID_USAGE_PAGE_GENERIC
    #define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
    #endif
    #ifndef HID_USAGE_GENERIC_MOUSE
    #define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
    #endif

    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    Rid[0].dwFlags = RIDEV_INPUTSINK;
    Rid[0].hwndTarget = window;
    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
    //---------------------

    // Begin Vulkan Setup

    return 0;
}

void SQ::GraphicsVulkan::BeginRender()
{
}

void SQ::GraphicsVulkan::UpdateProjectionMatrix(CameraNut* camera)
{
}

void SQ::GraphicsVulkan::SetupCameraForFrame(CameraNut* camera)
{
}

void SQ::GraphicsVulkan::Render(MeshNut* toRender)
{
}

void SQ::GraphicsVulkan::EndRender()
{
}

void SQ::GraphicsVulkan::RegisterLightForFrame(LightNut* light)
{
}

void SQ::GraphicsVulkan::ClearFrameLights()
{
}

SQ::Vec2 SQ::GraphicsVulkan::GetRenderWindowSize()
{
    return Vec2();
}

SQ::Vec2 SQ::GraphicsVulkan::GetWindowLocation()
{
    return Vec2();
}

#endif