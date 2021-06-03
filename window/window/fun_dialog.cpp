#include "fun_dialog.h"
#include <assert.h>
#include <memory>
#include "resource.h"

INT_PTR FunDialog::Dlgproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND button{ ::GetDlgItem(hwnd, IDC_CUSTOMBUTTON) };
        FunDialog* dialog{ reinterpret_cast<FunDialog*>(lParam) };
        dialog->mCustomButton.assignButton(button);
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
    FunDialog dialog;
    static_assert(sizeof(&dialog) == sizeof(LPARAM));
    // DialogBoxParamW is blocking, so it's ok to pass the pointer
    uint64_t value{ static_cast<uint64_t>(::DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_FUN_DIALOG), parent, Dlgproc, reinterpret_cast<LPARAM>(&dialog))) };

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
