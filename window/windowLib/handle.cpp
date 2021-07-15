#include "handle.h"
#include <assert.h>
#include <algorithm>

Handle::Handle(HANDLE arg)
    : mHandle{ arg }
{
}

Handle::~Handle()
{
    if (mHandle != INVALID_HANDLE_VALUE)
        ::CloseHandle(mHandle);
}

Handle::Handle(const Handle& arg)
{
    BOOL result{ ::DuplicateHandle(::GetCurrentProcess(), arg.mHandle, ::GetCurrentProcess(), &mHandle, 0,
        FALSE, DUPLICATE_SAME_ACCESS) };
    assert(result != FALSE);
}

Handle& Handle::operator=(const Handle& arg)
{
    if (mHandle != INVALID_HANDLE_VALUE)
        ::CloseHandle(mHandle);
    BOOL result{ ::DuplicateHandle(::GetCurrentProcess(), arg.mHandle, ::GetCurrentProcess(), &mHandle, 0,
        FALSE, DUPLICATE_SAME_ACCESS) };
    assert(result != FALSE);
    return *this;
}

Handle::Handle(Handle&& arg)
    : mHandle{arg.mHandle}
{
    arg.mHandle = INVALID_HANDLE_VALUE;
}

Handle& Handle::operator=(Handle&& arg)
{
    std::swap(arg.mHandle, mHandle);
    return *this;
}

Handle::operator HANDLE()
{
    return mHandle;
}
