#pragma once
#include <string>
#include <Windows.h>

namespace Registry
{

    std::wstring getStrKey(HKEY key, const std::wstring& subkey, const std::wstring& valueName);

}

//namespace Registry2
//{
//
//    std::wstring getStrKey(HKEY key, const std::wstring& subkey, const std::wstring& valueName)
//    {
//
//    }
//
//}