#ifdef _WIN32

#include <GL/glew.h>
#include <GL/wglew.h>

#include <stdexcept>

#include "rendercontext_wgl.h"
#include "log.h"

static bool InitGLEW();
static HGLRC CreateBasicContext(HDC);

bool RenderContextWGL::Create(void* osWnd) {
    window = static_cast<HWND>(osWnd);

    int glmajor = 3;
    int glminor = 3;
    int colorBits = 32;
    int depthBits = 24;
    int stencilBits = 0;
    int samples = 0;

    
    InitGLEW();

    int attrs[] = {
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? GL_TRUE : GL_FALSE,
        WGL_SAMPLES_ARB, samples,
        WGL_COLOR_BITS_ARB, colorBits,
        WGL_DEPTH_BITS_ARB, depthBits,
        WGL_STENCIL_BITS_ARB, stencilBits,
        WGL_ALPHA_BITS_ARB, colorBits == 32 ? 8 : 0,
        0
    };

    int pixelFormat;
    unsigned numFormats = 0;
    gdiDc = GetDC(window);

    int result = wglChoosePixelFormatARB(gdiDc, attrs, nullptr, 1, &pixelFormat, &numFormats);
    if (result == 0 || numFormats == 0) {
        Log::E("wglChoosePixelFormatARB failed");
    }
         
    // info
    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(gdiDc, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    Log::V() << "Pixel format:\tcColorBits " << (int)pfd.cColorBits << ", cDepthBits " << (int)pfd.cDepthBits
             << "\n\t\t\tcAlphaBits " << (int)pfd.cAlphaBits << ", cStencilBits " << (int)pfd.cStencilBits;

    if (!SetPixelFormat(gdiDc, pixelFormat, &pfd)) {
        Log::E("SetPixelFormat");
    }

    int contextAttrs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, glmajor,   //  OpenGL 4.3 provides full compatibility with OpenGL ES 3.0.
        WGL_CONTEXT_MINOR_VERSION_ARB, glminor,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,  // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
     // WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,   // WGL_CONTEXT_DEBUG_BIT_ARB
     // WGL_CONTEXT_LAYER_PLANE_ARB, 0,
        0
    };

    wglContext = wglCreateContextAttribsARB(gdiDc, 0, contextAttrs);
    if (!wglContext) {
        Log::E("wglContext");
    }

    if (!wglMakeCurrent(gdiDc, wglContext)) {
        Log::E("wglMakeCurrent");
        wglDeleteContext(wglContext);
        ReleaseDC(window, gdiDc);
        wglContext = nullptr;
        return false;
    }

    RECT rect;
    GetClientRect(window, &rect);
    mWidth = rect.right - rect.left;
    mHeight = rect.bottom - rect.top;

    return true;
}

static bool InitGLEW() {
    // dummy window
    HWND hwnd = CreateWindowA("STATIC", "",
        WS_POPUP | WS_DISABLED,
        -32000, -32000, 0, 0,
        nullptr, nullptr, GetModuleHandle(nullptr),
        0);

    if (!hwnd) {
        Log::E("Dummy window creation failed!");
        return false;
    }

    HDC hdc = GetDC(hwnd);
    HGLRC context = CreateBasicContext(hdc);
    if (!context) {
        Log::E("context");
        ReleaseDC(hwnd, hdc);
        return false;
    }

    GLenum error = glewInit();
    if (error != GLEW_OK) {
        Log::E("glew"); //throw std::runtime_error("GLEW init failed");        
        return false;
        //if (!GLEW_VERSION_3_3) throw std::runtime_error("Yay! OpenGL 3.3 is NOT supported!"); //  http://glew.sourceforge.net/basic.html
    }
    if (!WGLEW_ARB_pixel_format || !WGLEW_ARB_create_context || !WGLEW_ARB_extensions_string) {  // wglChoosePixelFormatARB, wglCreateContextAttribsARB, wglGetExtensionsStringARB
        Log::E("glew ARB");   //!wglewIsSupported("WGL_ARB_create_context")
        return false;
    }

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);        
    if (major < 3 || (major == 3 && minor < 3))
        Log::E("OpenGL 3.3 is not supported!");
    Log::V() << "GLEW initialized. Max OpenGL version supported: " << major << "." << minor;

    // -- glew setup so dispose of temp window -- ??
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(context);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd); // note: we have to destroy the window since attached pixel format is immutable

    return true;
}

static HGLRC CreateBasicContext(HDC hdc) {
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat(hdc, &pfd);
    if (!pf) {
        Log::E("ChoosePixelFormat");
        return nullptr;
    }

    //DescribePixelFormat(hdc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (!SetPixelFormat(hdc, pf, &pfd)) {
        Log::E("SetPixelFormat");
        return nullptr;
    }

    HGLRC context = wglCreateContext(hdc);

    if (!wglMakeCurrent(hdc, context)) {
        Log::E("wglMakeCurrent");
        wglDeleteContext(context);
        return nullptr;
    }

    return context;
}

void RenderContextWGL::Destroy() {
    if (wglContext) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(wglContext);
        ReleaseDC(window, gdiDc);
        wglContext = nullptr;
        gdiDc = nullptr;
        window = nullptr;
    }
}

void RenderContextWGL::Resize(int w, int h) {
    mWidth = w;
    mHeight = h;
}

void RenderContextWGL::Swap() {
    //wglMakeCurrent(gdiDc, wglContext);
    SwapBuffers(gdiDc);
}

void RenderContextWGL::SetVsync(bool enabled) {
    if (WGLEW_EXT_swap_control)  // == wglewIsSupported("WGL_EXT_swap_control")
        wglSwapIntervalEXT(enabled ? 1 : 0);
}

#endif