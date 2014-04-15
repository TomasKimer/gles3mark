#ifdef ANDROID

#pragma once

#include <EGL/egl.h>
#include "glcontext.h"

//TODO Unify HWND/ANativeWindow? - #include <EGL/eglplatform.h>, EGL for WIN?

class GLContextEGL : public GLContext {
	 EGLDisplay display;
	 EGLSurface surface;
	 EGLContext context;
	 ANativeWindow* window;

public:
    GLContextEGL() {};
    virtual ~GLContextEGL() {};

    /**
     * Initialize an EGL context for the current display.
     */
    virtual bool Create(void* osWnd) /*override*/;

    bool HasDisplay();

    /**
     * Tear down the EGL context currently associated with the display.
     */
    virtual void Destroy() /*override*/;

    virtual void Resize(int w, int h, bool vsync) /*override*/;
    virtual void Swap() /*override*/;
};

#endif
