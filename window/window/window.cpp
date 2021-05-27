#include "window.h"
#include <assert.h>
#include <limits>
#include <clrWrapper/wrapper.h>

MainWindow::MainWindow(HINSTANCE hInstance)
    : mWindow{
        (Window::registerClass(), hInstance),
        [this](Window& w) { onCreate(w); },
        [this](Window& w, UINT message, WPARAM wParam, LPARAM lParam) { return processMessage(w.getHwnd(), message, wParam, lParam); }
      }
{
}

void MainWindow::onCreate(Window& w)
{
    {
        ID2D1Factory* pD2DFactory{ nullptr };
        HRESULT hr = ::D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED,
            &pD2DFactory
        );
        assert(SUCCEEDED(hr));

        // Is a ComPtr
        mD2DFactory = pD2DFactory;

        // No need to release because ComPtr class takes care of it
        //pD2DFactory->Release();
    }
    {
        // Obtain the size of the drawing area.
        ::GetClientRect(w.getHwnd(), &mClientRect);

        // Create a Direct2D render target
        ID2D1HwndRenderTarget* pRT{ nullptr };
        HRESULT hr = mD2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(
                w.getHwnd(),
                D2D1::SizeU(
                    mClientRect.right - mClientRect.left,
                    mClientRect.bottom - mClientRect.top)
            ),
            &pRT
        );
        assert(SUCCEEDED(hr));
        mRenderTarget = pRT;
    }
    {
        ID2D1SolidColorBrush* pBlackBrush{ nullptr };
        mRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush
        );
        mBlackBrush = pBlackBrush;
    }

    auto value{ FunDialog::getValue(w.getHInstance(), w.getHwnd()) };
}

LRESULT MainWindow::processMessage(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        ::PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        mX = lParam & std::numeric_limits<uint16_t>::max();
        mY = lParam >> 0x10;
        break;
    }
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void MainWindow::frame()
{
    if (mMarginGrowing)
    {
        if (mMargin >= 65.f)
            mMarginGrowing = false;
        else
            mMargin += 1.f;
    }
    else
    {
        if (mMargin <= 5.f)
            mMarginGrowing = true;
        else
            mMargin -= 1.f;
    }

    mRenderTarget->BeginDraw();
    mRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::LightGreen));

    mRenderTarget->DrawRectangle(
        D2D1::RectF(
            mClientRect.left + mMargin,
            mClientRect.top + mMargin,
            mClientRect.right - mMargin,
            mClientRect.bottom - mMargin),
        mBlackBrush);

    float radius = getRadius(0);
    mRenderTarget->DrawEllipse(D2D1::Ellipse({ FLOAT(mX), FLOAT(mY) }, radius, radius), mBlackBrush);

    HRESULT hr = mRenderTarget->EndDraw();
}
