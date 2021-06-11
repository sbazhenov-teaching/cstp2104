#include "registry.h"
#include <assert.h>
#include <array>

namespace Registry
{

std::wstring getStrKey(HKEY key, const std::wstring& subkey, const std::wstring& valueName)
{
    HKEY hkey;
    {
        subkey.size();
        LSTATUS result{ ::RegOpenKeyEx(key, subkey.c_str(), 0, KEY_ALL_ACCESS, &hkey) };
        assert(result == ERROR_SUCCESS);
    }

    DWORD type;
    const unsigned bufSize{ 50 };
    //wchar_t dataBuf[bufSize];
    std::array<wchar_t, bufSize> dataBuf;
    static_assert(sizeof(dataBuf) == bufSize * sizeof(wchar_t));
    static_assert(sizeof(dataBuf) == 100);
    ::memset(dataBuf.data(), 0, sizeof(dataBuf));
    DWORD dataSize{ bufSize * sizeof(wchar_t) };
    LSTATUS result{ ::RegQueryValueEx(hkey, valueName.c_str(), nullptr, &type, reinterpret_cast<BYTE*>(dataBuf.data()), &dataSize) };
    assert(result == ERROR_SUCCESS);
    assert(type == REG_SZ);
    return dataBuf.data();
}

}