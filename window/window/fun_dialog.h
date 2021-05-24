#pragma once
#include <Windows.h>
#include <cstdint>

class FunDialog
{
public:
    static uint64_t getValue(HINSTANCE, HWND);
private:
    static INT_PTR Dlgproc(HWND, UINT, WPARAM, LPARAM);
};