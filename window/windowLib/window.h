#pragma once
#include <Windows.h>
#include <functional>

class Window
{
public:
    static void registerClass();
    static LRESULT CALLBACK
        windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    using OnCreate = std::function<void(Window&)>;
    using OnMessage = std::function<LRESULT(
        Window&, UINT message, WPARAM wParam, LPARAM lParam)>;

    Window(HINSTANCE hInstance, OnCreate, OnMessage);

    HINSTANCE getHInstance() const;
    HWND getHwnd() const;
protected:
    OnCreate mOnCreate;
    OnMessage mOnMessage;
    HINSTANCE mHInstance;
    HWND mHwnd{ nullptr };
};

