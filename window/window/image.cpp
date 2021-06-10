#include "image.h"
#include "resource.h"
#include <assert.h>
#include <windowLib/comPtr.h>

ComPtr<ID2D1Bitmap> loadImage(ID2D1HwndRenderTarget* rt)
{
    HRSRC picResHandle{ ::FindResource(NULL, MAKEINTRESOURCE(IDB_BITMAP1), L"JPG") };

    {
        DWORD err{ ::GetLastError() };
        //void* p = ::malloc(5);
        //::free(p);
        LPWSTR messageBuffer{ nullptr };
        DWORD size{ ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&messageBuffer), 0, NULL) };
        ::LocalFree(messageBuffer);
    }

    assert(picResHandle);

    HGLOBAL imageResDataHandle{ ::LoadResource(NULL, picResHandle) };
    assert(imageResDataHandle);

    void* picData{ ::LockResource(imageResDataHandle) };
    assert(picData);

    DWORD picSize{ ::SizeofResource(NULL, picResHandle) };
    assert(picSize);

    ComPtr<IWICImagingFactory> factory;
    // Illegal if there is no copy constructor
    //ComPtr<IWICImagingFactory> factory2{ factory };
    {
        IWICImagingFactory* pFactory = NULL;
        // Create the COM imaging factory
        HRESULT hr{ ::CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pFactory)
        ) };
        assert(SUCCEEDED(hr));
        factory = pFactory;
    }

    ComPtr<IWICStream> stream;
    {
        IWICStream* pIWICStream;
        HRESULT hr{ factory->CreateStream(&pIWICStream) };
        assert(SUCCEEDED(hr));
        stream = pIWICStream;
    }

    HRESULT hr{ stream->InitializeFromMemory(
        reinterpret_cast<BYTE*>(picData),
        picSize) };
    assert(SUCCEEDED(hr));

    ComPtr<IWICBitmapDecoder> decoder;
    {
        IWICBitmapDecoder* pIDecoder;
        hr = factory->CreateDecoderFromStream(
            stream,
            NULL, // Do not prefer a particular vendor
            WICDecodeMetadataCacheOnLoad,  // Cache metadata when needed
            &pIDecoder);
        assert(SUCCEEDED(hr));
        decoder = pIDecoder;
    }

    {
        UINT frameCount;
        decoder->GetFrameCount(&frameCount);
        assert(frameCount == 1);
    }

    ComPtr<IWICBitmapFrameDecode> frame;
    {
        IWICBitmapFrameDecode* pIDecoderFrame;
        HRESULT hr{ decoder->GetFrame(0, &pIDecoderFrame) };
        assert(SUCCEEDED(hr));
        frame = pIDecoderFrame;
    }

    ComPtr<IWICFormatConverter> converter;
    {
        IWICFormatConverter* pConverter;
        HRESULT hr{ factory->CreateFormatConverter(&pConverter) };
        assert(SUCCEEDED(hr));
        converter = pConverter;

        hr = converter->Initialize(
            frame,
            GUID_WICPixelFormat32bppPBGRA,  // format suitable for Direct2D
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut
        );
        assert(SUCCEEDED(hr));
    }

    {
        ID2D1Bitmap* ppBitmap;
        HRESULT hr{ rt->CreateBitmapFromWicBitmap(
            converter,
            NULL,
            &ppBitmap
            ) };
        assert(SUCCEEDED(hr));
        return ppBitmap;
    }
}