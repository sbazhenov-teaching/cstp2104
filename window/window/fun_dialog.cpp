#include "fun_dialog.h"
#include <assert.h>
#include <memory>
#include "resource.h"

INT_PTR FunDialog::Dlgproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        case IDCANCEL:
            ::EndDialog(hwnd, 10);
            return TRUE;
        }
    case WM_DESTROY:
        return TRUE;
    }
    return FALSE;
}

uint64_t FunDialog::getValue(HINSTANCE hInstance, HWND parent)
{
    static_assert(sizeof(INT_PTR) == sizeof(unsigned long long));
    uint64_t value{ static_cast<uint64_t>(::DialogBox(hInstance, MAKEINTRESOURCE(IDD_FUN_DIALOG), parent, Dlgproc)) };

    return value;
    //if (!hwnd)
    //{
    //    DWORD err{ ::GetLastError() };

    //    LPWSTR messageBuffer{ nullptr };
    //    DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    //        NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
    //    ::LocalFree(messageBuffer);
    //}
    //BOOL wasPrevShowed{ ::ShowWindow(hwnd, SW_SHOW) };
    //assert(wasPrevShowed == FALSE);
}
