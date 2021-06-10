#include "window.h"
#include <assert.h>
#include <limits>

const wchar_t* cWndClassName{ L"ExampleWinApp" };

LRESULT CALLBACK
Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Window* window =
        reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (window)
    {
        return window->mOnMessage(*window, message, wParam, lParam);
    }
    else if (message == WM_CREATE)
    {
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        window = reinterpret_cast<Window*>(createStruct->lpCreateParams);

        ::SetLastError(0);
        const LONG_PTR prevValue = ::SetWindowLongPtr(
            hWnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(createStruct->lpCreateParams));
        const DWORD lastError = ::GetLastError();
        assert(prevValue != 0 || lastError == 0);

        window->mHwnd = hWnd;
        window->mOnCreate(*window);

        return 0;
    }
    else
    {
        return ::DefWindowProc(hWnd, message, wParam, lParam);
    }
}

HINSTANCE Window::getHInstance() const
{
    return mHInstance;
}

HWND Window::getHwnd() const
{
    return mHwnd;
}

Window::Window(HINSTANCE hInstance, OnCreate onCreate, OnMessage onMessage, const std::wstring& caption)
    : mOnCreate{ std::move(onCreate) }
    , mOnMessage{ std::move(onMessage) }
    , mHInstance{ hInstance }
{
    // || - or
    // | - bitwise or
    ::CreateWindowEx(
        0,
        cWndClassName,
        caption.c_str(),
        (WS_VISIBLE | WS_OVERLAPPEDWINDOW) & ~WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        nullptr, // parent window
        nullptr, // menu
        hInstance,
        this);
}

void Window::registerClass()
{
    HINSTANCE module = ::GetModuleHandle(NULL);

    WNDCLASSEX windowClass = { 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.lpfnWndProc = windowProc;
    windowClass.hInstance = module;
    windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = cWndClassName;
    const ATOM registered = ::RegisterClassEx(&windowClass);
    if(!registered)
    {
        DWORD err{ ::GetLastError() };
        assert(err == ERROR_CLASS_ALREADY_EXISTS);

        LPWSTR messageBuffer{ nullptr };
        DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
        ::LocalFree(messageBuffer);
    }
}
