#ifdef ANDROID

#pragma once

#include <EGL/egl.h>
#include "glcontext.h"

//TODO Unify HWND/ANativeWindow? - #include <EGL/eglplatform.h>, EGL for WIN?

class GL3ContextEGL : public GLContext {
	 EGLDisplay display;
	 EGLSurface surface;
	 EGLContext context;
	 ANativeWindow* window;

public:
    GL3ContextEGL() {};
    virtual ~GL3ContextEGL() {};

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
