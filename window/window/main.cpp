#include "mainWindow.h"
#include <assert.h>

// Standard entry point
//int main()
//{
//    return 0;
//}

// good way:
//const int a = 5;
// dangerous way:
//#define a 5

// function type
//typedef void(fnA)(int);
// function type - same, but modern
//using fnA = void(int);

// Windows entry point
int WINAPI WinMain(
    _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    // Initialize COM
    {
        HRESULT hr{ ::CoInitialize(NULL) };
        assert(hr == S_OK);
    }

    MainWindow window(hInstance);

    // #define replacement:
    //int a;
    //int 5;

    MSG msg = {};
    int counter = 0;
    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
        else
        {
            window.frame();
        }
    }

    return 0;
}