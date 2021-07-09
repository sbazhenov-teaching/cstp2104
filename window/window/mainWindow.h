#pragma once
#include "server.h"
#include <Windows.h>
#include <d2d1.h>
#include <cstdint>
#include <windowLib/comPtr.h>
#include <windowLib/window.h>
#include "fun_dialog.h"
#include "input.h"
#include <thread>
#include <mutex>
#include <windowLib/threadPool.h>

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
    void networkThread();
    void getFromClient();

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

    ServerApp::Server mServer;

    // Initialization order matters!
    std::thread mCircleThread;
    std::thread mNetworkThread;
    ThreadPool<2> mThreadPool;
    Window mWindow;

    Input mInput;
};