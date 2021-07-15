#pragma once
#include <Windows.h>

class Handle
{
public:
    Handle() = default;
    Handle(HANDLE);
    ~Handle();
    Handle(const Handle&);
    Handle& operator = (const Handle&);
    Handle(Handle&&);
    Handle& operator = (Handle&&);
    operator HANDLE();
private:
    HANDLE mHandle{ INVALID_HANDLE_VALUE };
};

