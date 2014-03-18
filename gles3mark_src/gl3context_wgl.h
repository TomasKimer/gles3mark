#ifdef _WIN32

#pragma once

#include "glcontext.h"

#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32.lib")

class GL3ContextWGL : public GLContext {
    
    HWND    window;
    HGLRC   wglContext;
    HDC     gdiDc;

public:
    GL3ContextWGL(): window(nullptr), wglContext(nullptr), gdiDc(nullptr) {};
    virtual ~GL3ContextWGL() {};

    virtual bool Create(void* osWnd) override;
    virtual void Destroy() override;    
    virtual void Resize(int w, int h, bool vsync) override;    
    virtual void Swap() override;
};

#endif
