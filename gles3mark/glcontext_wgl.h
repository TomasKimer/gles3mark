#ifdef _WIN32

#pragma once

#include "glcontext.h"

#include <windows.h>

class GLContextWGL : public GLContext {
    
    HWND    window;
    HGLRC   wglContext;
    HDC     gdiDc;

public:
    GLContextWGL(): window(nullptr), wglContext(nullptr), gdiDc(nullptr) {};
    virtual ~GLContextWGL() {};

    virtual bool Create(void* osWnd) override;
    virtual void Destroy() override;    
    virtual void Resize(int w, int h, bool vsync) override;    
    virtual void Swap() override;
};

#endif
