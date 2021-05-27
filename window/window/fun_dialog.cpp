#include "fun_dialog.h"
#include <assert.h>
#include <memory>
#include <commctrl.h>
#include "resource.h"

LRESULT CALLBACK OwnerDrawButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (uMsg)
    {
    case WM_PAINT:

            return TRUE;
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

INT_PTR FunDialog::Dlgproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND button{ ::GetDlgItem(hwnd, IDC_CUSTOMBUTTON) };

        LONG style{ ::GetWindowLong(button, GWL_STYLE) };
        style |= BS_OWNERDRAW;
        ::SetWindowLong(button, GWL_STYLE, style);

        ::SetWindowSubclass(button, OwnerDrawButtonProc, 0, 0);
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) // Control identifier
        {
        case IDOK:
        case IDCANCEL:
            ::EndDialog(hwnd, 10);
            return TRUE;
        case IDC_CUSTOMBUTTON:
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
