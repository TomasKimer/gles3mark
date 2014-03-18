#ifdef _WIN32

#include "gl3context_wgl.h"

bool GL3ContextWGL::Create(void* osWnd) {
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
        wglContext = nullptr;
        return false;
    }

    /*glewInit();
    if (WGLEW_EXT_swap_control)     // if (wglewIsSupported("WGL_EXT_swap_control"))
    wglSwapIntervalEXT(0);*/

    // !!! TODO !!!
    //if (GLEW_VERSION_3_3) {
    /* Yay! OpenGL 3.3 is supported! */
    // http://glew.sourceforge.net/basic.html
    //}

    return true;
}

void GL3ContextWGL::Destroy() {
    if (wglContext) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(wglContext);
        ReleaseDC(window, gdiDc);
        wglContext = nullptr;
        gdiDc = nullptr;
        window = nullptr;
    }
}

void GL3ContextWGL::Resize(int w, int h, bool vsync) {
    mWidth = w;
    mHeight = h;

    if (WGLEW_EXT_swap_control)
        wglSwapIntervalEXT(vsync ? 1 : 0);
}

void GL3ContextWGL::Swap() {
    SwapBuffers(gdiDc);
}

#endif