#pragma once
#include <Windows.h>
#include <functional>

class Window
{
public:
    static void registerClass(const wchar_t* wndClassName);
    static LRESULT CALLBACK
        windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    using OnCreate = std::function<void(Window&)>;
    using OnMessage = std::function<LRESULT(
        Window&, UINT message, WPARAM wParam, LPARAM lParam)>;

    Window(const wchar_t* wndClassName, HINSTANCE hInstance, OnCreate, OnMessage, const std::wstring& caption);

    HINSTANCE getHInstance() const;
    HWND getHwnd() const;
protected:
    OnCreate mOnCreate;
    OnMessage mOnMessage;
    HINSTANCE mHInstance;
    HWND mHwnd{ nullptr };
};

