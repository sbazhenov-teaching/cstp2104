#include "customButton.h"
#include <commctrl.h>
#include <assert.h>

void CustomButton::assignButton(HWND button)
{
    LONG style{ ::GetWindowLong(button, GWL_STYLE) };
    style |= BS_OWNERDRAW;
    ::SetWindowLong(button, GWL_STYLE, style);

    const LONG_PTR prevValue{ ::SetWindowLongPtr(
        button,
        GWLP_USERDATA,
        reinterpret_cast<LONG_PTR>(this)) };
    mHwnd = button;
    ::GetClientRect(button, &mClientRect);

    ::SetWindowSubclass(button, CustomButton::buttonProc, 0, 0);
}

LRESULT CALLBACK CustomButton::buttonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    CustomButton* button{
        reinterpret_cast<CustomButton*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA)) };
    return button->processMsg(uMsg, wParam, lParam);
}

LRESULT CustomButton::processMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        /*RECT rect;
        ::GetClientRect(mHwnd, &rect);*/
        HDC hdc{ ::GetDC(mHwnd) };
        HPEN hpenDot{ ::CreatePen(PS_DASHDOTDOT, 5, RGB(250, 90, 60)) };
        ::SelectObject(hdc, hpenDot);
        ::Rectangle(hdc, mClientRect.left, mClientRect.top, mClientRect.right, mClientRect.bottom);
        ::ReleaseDC(mHwnd, hdc);
        ::DeleteObject(hpenDot);
        return TRUE;
    }

    }
    return ::DefSubclassProc(mHwnd, uMsg, wParam, lParam);
}
