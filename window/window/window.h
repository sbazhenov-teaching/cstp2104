#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <cstdint>
#include <windowLib/comPtr.h>
#include <windowLib/window.h>
#include "fun_dialog.h"

class MainWindow
{
public:
    MainWindow(HINSTANCE hInstance);

    void frame();
private:
    void onCreate(Window&);
    LRESULT processMessage(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    RECT mClientRect;
    ComPtr<ID2D1Factory> mD2DFactory;
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;
    ComPtr<ID2D1SolidColorBrush> mBlackBrush;

    float mMargin{ 5.0f };
    bool mMarginGrowing{ true };

    uint32_t mX{ 0 };
    uint32_t mY{ 0 };

    // Initialization order matters!
    Window mWindow;
};