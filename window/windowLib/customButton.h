#pragma once
#include <Windows.h>

class CustomButton
{
public:
    static void assignButton(HWND);
private:
    static LRESULT CALLBACK buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
        LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
};
