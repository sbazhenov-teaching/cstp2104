#include "input.h"
#include <hidusage.h>
#include <assert.h>

const USHORT VKEY_W{ 0x57 };
const USHORT VKEY_A{ 0x41 };
const USHORT VKEY_S{ 0x53 };
const USHORT VKEY_D{ 0x44 };

void Input::init(HWND hwnd)
{
    RAWINPUTDEVICE devices[3];

    devices[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
    devices[0].usUsage = HID_USAGE_GENERIC_MOUSE;
    devices[0].dwFlags = RIDEV_DEVNOTIFY;
    devices[0].hwndTarget = hwnd;

    devices[1].usUsagePage = HID_USAGE_PAGE_GENERIC;
    devices[1].usUsage = HID_USAGE_GENERIC_KEYBOARD;
    devices[1].dwFlags = RIDEV_NOLEGACY | RIDEV_DEVNOTIFY;
    devices[1].hwndTarget = hwnd;

    devices[2].usUsagePage = HID_USAGE_PAGE_GENERIC;
    devices[2].usUsage = HID_USAGE_GENERIC_GAMEPAD;
    devices[2].dwFlags = RIDEV_DEVNOTIFY;
    devices[2].hwndTarget = hwnd;

    const BOOL registered = ::RegisterRawInputDevices(
        devices, 3, sizeof(RAWINPUTDEVICE));
    assert(registered == TRUE);
}

Key Input::process(LPARAM lParam)
{
    RAWINPUT input;
    {
        UINT size;

        const UINT probeResult{ ::GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            nullptr,
            &size,
            sizeof(RAWINPUTHEADER)) };
        assert(probeResult == 0);
        assert(size <= sizeof(input));

        const UINT result{ ::GetRawInputData(
            reinterpret_cast<HRAWINPUT>(lParam),
            RID_INPUT,
            &input,
            &size,
            sizeof(RAWINPUTHEADER)) };
        assert(result <= sizeof(RAWINPUT));
    }

    switch (input.header.dwType)
    {
    case RIM_TYPEMOUSE:
        break;
    case RIM_TYPEKEYBOARD:
        if (input.data.keyboard.Message == WM_KEYDOWN ||
            input.data.keyboard.Message == WM_SYSKEYDOWN)
        {
            switch (input.data.keyboard.VKey)
            {
            case VK_UP:
            case VKEY_W:
                return Key::Up;
            case VK_DOWN:
            case VKEY_S:
                return Key::Down;
            case VK_LEFT:
            case VKEY_A:
                return Key::Left;
            case VK_RIGHT:
            case VKEY_D:
                return Key::Right;
            case VK_SPACE:
                return Key::Space;
            }
        }
        else if (
            input.data.keyboard.Message == WM_KEYUP ||
            input.data.keyboard.Message == WM_SYSKEYUP)
        {
            //
        }
        break;
    case RIM_TYPEHID: //gamepad
        break;
    }

    return Key::None;
}
