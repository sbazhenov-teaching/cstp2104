#pragma once
#include <Windows.h>
#include <cstdint>

class CustomButton
{
public:
    void assignButton(HWND);

private:
    static LRESULT CALLBACK buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
        LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

    LRESULT processMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND mHwnd;
    uint32_t mX;
    uint32_t mY;
    RECT mRect;
};
