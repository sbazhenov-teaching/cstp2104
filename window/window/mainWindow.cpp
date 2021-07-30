#include "mainWindow.h"
#include <assert.h>
#include <limits>
#include <clrWrapper/wrapper.h>
#include "image.h"
#include "registry.h"
//#include "server.h"
//#include <networkLib/addrInfo.h>
#include <assert.h>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
const wchar_t* cWndClassName{ L"ExampleWinApp" };

//class Lambda
//{
//public:
//    Lambda(MainWindow* ptr) : mPtr{ ptr } {}
//    void operator()(Window& w) { mPtr->onCreate(w); }
//private:
//    MainWindow* mPtr;
//};

MainWindow::MainWindow(HINSTANCE hInstance)
    : mServer{ hInstance },
    mWindow{
        cWndClassName,
        (Window::registerClass(cWndClassName), hInstance),
        [this](Window& w) { onCreate(w); },
        [this](Window& w, UINT message, WPARAM wParam, LPARAM lParam) { return processMessage(w.getHwnd(), message, wParam, lParam); },
        Registry::getStrKey(HKEY_CURRENT_USER, L"SOFTWARE\\CSTP2104\\WindowApp\0asdfasdf", L"Caption")
      }
{
    //Lambda l(this);
    //l(mWindow);
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

    mBitmap = loadImage(mRenderTarget);

    mInput.init(mWindow.getHwnd());

    mCircleThread = std::thread([this]() {
        // The same
        //std::this_thread::sleep_for(std::chrono::seconds(15));
        //::Sleep(5000);

        circleThread();
    });

    mNetworkThread = std::thread([this]() {
        networkThread();
    });

    //auto value{ FunDialog::getValue(w.getHInstance(), w.getHwnd()) };
}

LRESULT MainWindow::processMessage(
    HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CLOSE:
        mStopping = true;
        assert(mCircleThread.joinable());
        mCircleThread.join();
        mServer.stop();
        assert(mNetworkThread.joinable());
        mNetworkThread.join();
        ::PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        //mX = lParam & std::numeric_limits<uint16_t>::max();
        //mY = lParam >> 0x10;
        break;
    case WM_INPUT:
    {
        Key key{ mInput.process(lParam) };
        //std::lock_guard<std::mutex> lock(mCircleMutex);
        processKey(key);
    }
        break;
    }
    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void MainWindow::processKey(Key key)
{
    std::unique_lock<std::mutex> lock(mCircleMutex);
    switch (key)
    {
    case Key::Right:
        mX += mStep;
        break;
    case Key::Left:
        mX -= mStep;
        break;
    case Key::Down:
        mY += mStep;
        break;
    case Key::Up:
        mY -= mStep;
        break;
    case Key::Space:
        //mThreadPool.post([]() { std::this_thread::sleep_for(std::chrono::seconds(10)); });
        //mThreadPool.post([this]() { getFromClient(); });
        break;
    }
}

void MainWindow::setStep(unsigned arg)
{
    std::unique_lock<std::mutex> lock(mCircleMutex);
    mStep = arg;
}

void MainWindow::circleThread()
{
    while (!mStopping)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(33));

        std::unique_lock<std::mutex> lock(mCircleMutex);
        //mX += 1;
        //mCircleMutex.lock();
        //// do whatever - exclusive access guaranteed
        //mCircleMutex.unlock();
    }
}

void MainWindow::networkThread()
{
    mServer.init(
        [this](unsigned short arg) { processKey(static_cast<Key>(arg)); },
        [this](unsigned arg) { setStep(arg); });
    while (!mStopping)
    {
        mServer.serve();
    }
}

void MainWindow::getFromClient()
{
    //Server::serve();
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

    {
        D2D1_RECT_F rectf;
        rectf.top = 0.f;
        rectf.left = 0.f;
        rectf.right = 300.f;
        rectf.bottom = 600.f;
        D2D1_RECT_F rectfSource;
        rectfSource.top = 0.f;
        rectfSource.left = 0.f;
        rectfSource.right = mBitmap->GetSize().width / 2;
        rectfSource.bottom = mBitmap->GetSize().height;
        mRenderTarget->DrawBitmap(mBitmap, rectf, 0.7, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, rectfSource);
    }

    float radius = getRadius(0);
    {
        std::lock_guard<std::mutex> lock(mCircleMutex);
        mRenderTarget->DrawEllipse(D2D1::Ellipse({ FLOAT(mX), FLOAT(mY) }, radius, radius), mBlackBrush);
    }

    HRESULT hr = mRenderTarget->EndDraw();
}
