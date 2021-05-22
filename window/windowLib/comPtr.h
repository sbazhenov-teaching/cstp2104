#pragma once

template<typename T>
class ComPtr
{
public:
    ComPtr& operator = (T* arg)
    {
        if (mPtr)
            mPtr->Release();
        mPtr = arg;
        return *this;
    }

    ~ComPtr()
    {
        if (mPtr)
            mPtr->Release();
    }

    T* operator->()
    {
        return mPtr;
    }

    operator T* ()
    {
        return mPtr;
    }
private:
    T* mPtr{ nullptr };
};