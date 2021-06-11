#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <cstdint>
#include <windowLib/comPtr.h>
#include <windowLib/window.h>
#include "fun_dialog.h"
#include "input.h"
#include <thread>
#include <mutex>

class MainWindow
{
public:
    MainWindow(HINSTANCE hInstance);

    void frame();
private:
    void onCreate(Window&);
    LRESULT processMessage(
        HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void circleThread();

    RECT mClientRect;
    ComPtr<ID2D1Factory> mD2DFactory;
    ComPtr<ID2D1HwndRenderTarget> mRenderTarget;
    ComPtr<ID2D1SolidColorBrush> mBlackBrush;
    ComPtr<ID2D1Bitmap> mBitmap;

    float mMargin{ 5.0f };
    bool mMarginGrowing{ true };

    uint32_t mX{ 0 };
    uint32_t mY{ 0 };
    std::mutex mCircleMutex;
    bool mStopping{ false };

    // Initialization order matters!
    std::thread mCircleThread;
    Window mWindow;

    Input mInput;
};