#pragma once

#include <wincodec.h>
#include <d2d1.h>
#include <windowLib/comPtr.h>

ComPtr<ID2D1Bitmap> loadImage(ID2D1HwndRenderTarget*);
