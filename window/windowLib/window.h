#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <cstdint>

class Window
{
public:
    static void registerClass();
    static LRESULT CALLBACK
        windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    Window(HINSTANCE hInstance);

    //void frame();
private:
    //void onCreate(HWND hWnd);
    //LRESULT processMessage(
    //    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    //void GetModuleHandle(void*);

    //int mDemo{ 2 };
    //HWND mHwnd{ nullptr };
    //RECT mClientRect;
    //ComPtr<ID2D1Factory> mD2DFactory;
    //ComPtr<ID2D1HwndRenderTarget> mRenderTarget;
    //ComPtr<ID2D1SolidColorBrush> mBlackBrush;

    //float mMargin{ 5.0f };
    //bool mMarginGrowing{ true };

    //uint32_t mX{ 0 };
    //uint32_t mY{ 0 };
};