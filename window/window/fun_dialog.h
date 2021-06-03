#pragma once
#include <Windows.h>
#include <cstdint>
#include <windowLib/customButton.h>

class FunDialog
{
public:
    static uint64_t getValue(HINSTANCE, HWND);
private:
    static INT_PTR Dlgproc(HWND, UINT, WPARAM, LPARAM);

    CustomButton mCustomButton;
};