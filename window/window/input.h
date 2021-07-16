#pragma once
#include <Windows.h>
#include <windowLib/keys.h>

class Input
{
public:
    void init(HWND hwnd);
    Key process(LPARAM lParam);
private:

};
