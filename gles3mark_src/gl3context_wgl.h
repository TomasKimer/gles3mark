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
    GL3ContextWGL(): window(NULL), wglContext(NULL), gdiDc(NULL) { };
    virtual ~GL3ContextWGL() {};

    virtual bool Create(void* osWnd) {
        window = (HWND)osWnd;

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));

        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 16;

        gdiDc = GetDC(window);
        int pf = ChoosePixelFormat(gdiDc, &pfd);
        if (!pf) {
            ReleaseDC(window, gdiDc);
            return false;
        }
        if (!SetPixelFormat(gdiDc, pf, &pfd)) {
            ReleaseDC(window, gdiDc);
            return false;
        }
        wglContext = wglCreateContext(gdiDc);
        if (!wglMakeCurrent(gdiDc, wglContext)) {
            wglDeleteContext(wglContext);
            ReleaseDC(window, gdiDc);
            wglContext = NULL;
            return false;
        }

        /*glewInit();
        if (WGLEW_EXT_swap_control)     // if (wglewIsSupported("WGL_EXT_swap_control"))
            wglSwapIntervalEXT(0);*/
        //if (GLEW_VERSION_3_3) {
            /* Yay! OpenGL 1.3 is supported! */
            // http://glew.sourceforge.net/basic.html
        //}
        
        return true;
    }

    virtual void Destroy() {
        if (wglContext) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(wglContext);
            ReleaseDC(window, gdiDc);
            wglContext = NULL;
            gdiDc = NULL;
            window = NULL;
        }    
    }
    
    virtual void Resize(int w, int h, bool vsync) {
        mWidth = w;
        mHeight = h;

    	if (WGLEW_EXT_swap_control)
            wglSwapIntervalEXT(vsync ? 1 : 0);    
    }
    
    virtual void Swap() {
        SwapBuffers(gdiDc);
    }
};

#endif
