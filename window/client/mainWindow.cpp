#include <winsock2.h>
#include "mainWindow.h"
#include "clientConnection.h"
#include <stdexcept>
#include <string>

ClientConnection gClientConnection;

void sendKey(Key key)
{
    gClientConnection.sendToServer(Command::Direction);
    gClientConnection.sendToServer(key);
}

INT_PTR MainDialog::dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND button{::GetDlgItem(hwnd, IDC_BUTTON_UP)};
        MainDialog *dialog{reinterpret_cast<MainDialog *>(lParam)};
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
            sendKey(Key::Up);
            //::EndDialog(hwnd, 10);
            return TRUE;
        case IDC_BUTTON_DOWN:
            sendKey(Key::Down);
            return TRUE;
        case IDC_BUTTON_LEFT:
            sendKey(Key::Left);
            return TRUE;
        case IDC_BUTTON_RIGHT:
            sendKey(Key::Right);
            return TRUE;
        case IDC_BUTTON_STEPS:
        {
            // FIXME
            gClientConnection.sendToServer(Command::Step);
            gClientConnection.sendToServer(1);
            //std::wstring str;
            //str.resize(10);
            //UINT strSize{::GetDlgItemText(hwnd, IDC_EDIT1, str.data(), str.size())};
            //try
            //{
            //    int steps{std::stoi(str)};
            //    gClientConnection.sendToServer(steps);
            //}
            //catch (const std::exception &e)
            //{
            //    ::OutputDebugString(L"Invalid value");
            //}
            return TRUE;
        }
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
    uint64_t value{static_cast<uint64_t>(::DialogBoxParamW(
        hInstance,
        MAKEINTRESOURCE(IDD_DIALOG1),
        parent,
        dlgProc,
        reinterpret_cast<LPARAM>(&dialog)))};

    return value;
    // if (!hwnd)
    //{
    //    DWORD err{ ::GetLastError() };

    //    LPWSTR messageBuffer{ nullptr };
    //    DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
    //    FORMAT_MESSAGE_IGNORE_INSERTS,
    //        NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    //        reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
    //    ::LocalFree(messageBuffer);
    //}
    // BOOL wasPrevShowed{ ::ShowWindow(hwnd, SW_SHOW) };
    // assert(wasPrevShowed == FALSE);
}