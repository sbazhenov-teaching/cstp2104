#pragma once

#include <windowLib/window.h>
#include "resource.h"

class MainDialog
{
public:
    static uint64_t getValue(HINSTANCE, HWND);
private:
    static INT_PTR dlgProc(HWND, UINT, WPARAM, LPARAM);
};