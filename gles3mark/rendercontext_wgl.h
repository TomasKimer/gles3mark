#ifdef _WIN32

#pragma once

#include "rendercontext.h"

#include <windows.h>

class RenderContextWGL : public RenderContext {
    
    HWND    window;
    HGLRC   wglContext;
    HDC     gdiDc;

public:
    RenderContextWGL(): window(nullptr), wglContext(nullptr), gdiDc(nullptr) {};
    virtual ~RenderContextWGL() {};

    virtual bool Create(void* osWnd) override;
    virtual void Destroy() override;    
    virtual void Resize(int w, int h, bool vsync) override;    
    virtual void Swap() override;
};

#endif
