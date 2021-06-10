#pragma once
#include <Windows.h>

class Input
{
public:
    enum class Key
    {
        Left,
        Right,
        Up,
        Down,
        None
    };

    void init(HWND hwnd);
    Key process(LPARAM lParam);
private:

};
