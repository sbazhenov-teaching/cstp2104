#include "customButton.h"
#include <commctrl.h>

void CustomButton::assignButton(HWND button)
{
    LONG style{ ::GetWindowLong(button, GWL_STYLE) };
    style |= BS_OWNERDRAW;
    ::SetWindowLong(button, GWL_STYLE, style);

    const LONG_PTR prevValue{ ::SetWindowLongPtr(
        button,
        GWLP_USERDATA,
        reinterpret_cast<LONG_PTR>(this)) };

    ::SetWindowSubclass(button, CustomButton::buttonProc, 0, 0);
}

LRESULT CALLBACK CustomButton::buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
    case WM_PAINT:
        CustomButton* button =
            reinterpret_cast<CustomButton*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

        RECT rect;
        ::GetClientRect(hWnd, &rect);
        HDC hdc{ ::GetDC(hWnd) };
        HPEN hpenDot{ ::CreatePen(PS_DASHDOTDOT, 5, RGB(250, 90, 60)) };
        ::SelectObject(hdc, hpenDot);
        ::Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        ::ReleaseDC(hWnd, hdc);
        ::DeleteObject(hpenDot);
        return TRUE;
    }
    return ::DefSubclassProc(hWnd, uMsg, wParam, lParam);
}
