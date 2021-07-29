#include "clientConnection.h"
#include "mainWindow.h"

ClientConnection gClientConnection;

INT_PTR MainDialog::Dlgproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND button{ ::GetDlgItem(hwnd, IDC_BUTTON_UP) };
        MainDialog* dialog{ reinterpret_cast<MainDialog*>(lParam) };
        gClientConnection.init();
        return TRUE;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) // Control identifier
        {
        case IDOK:
        case IDCANCEL:
            ::EndDialog(hwnd, 10);
            return TRUE;
        case IDC_BUTTON_UP:
            gClientConnection.sendToServer(Key::Up);
            //::EndDialog(hwnd, 10);
            return TRUE;
        case IDC_BUTTON_DOWN:
            gClientConnection.sendToServer(Key::Down);
            return TRUE;
        case IDC_BUTTON_LEFT:
            gClientConnection.sendToServer(Key::Left);
            return TRUE;
        case IDC_BUTTON_RIGHT:
            gClientConnection.sendToServer(Key::Right);
            return TRUE;
        }
    case WM_DESTROY:
        gClientConnection.shutdown();
        return TRUE;
    }
    return FALSE;
}

uint64_t MainDialog::getValue(HINSTANCE hInstance, HWND parent)
{
    MainDialog dialog;
    static_assert(sizeof(&dialog) == sizeof(LPARAM));
    // DialogBoxParamW is blocking, so it's ok to pass the pointer
    uint64_t value{ static_cast<uint64_t>(::DialogBoxParamW(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), parent, Dlgproc, reinterpret_cast<LPARAM>(&dialog))) };

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