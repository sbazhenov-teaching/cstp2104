#include "registry.h"
#include <assert.h>

namespace Registry
{

std::wstring getStrKey(HKEY key, const std::wstring& subkey, const std::wstring& valueName)
{
    HKEY hkey;
    {
        LSTATUS result{ ::RegOpenKeyEx(key, subkey.c_str(), 0, KEY_ALL_ACCESS, &hkey) };
        assert(result == ERROR_SUCCESS);
    }

    DWORD type;
    const unsigned bufSize{ 50 };
    wchar_t dataBuf[bufSize];
    static_assert(sizeof(dataBuf) == bufSize * sizeof(wchar_t));
    ::memset(dataBuf, 0, sizeof(dataBuf));
    DWORD dataSize{ bufSize };
    LSTATUS result{ ::RegQueryValueEx(hkey, valueName.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(dataBuf), &dataSize) };
    assert(result == ERROR_SUCCESS);
    assert(type == REG_SZ);
    return dataBuf;
}

}