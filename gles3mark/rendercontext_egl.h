/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#ifdef ANDROID

#pragma once

#include <EGL/egl.h>
#include "rendercontext.h"

//TODO Unify HWND/ANativeWindow? - #include <EGL/eglplatform.h>, EGL for WIN?

class RenderContextEGL : public RenderContext {
	 EGLDisplay display;
	 EGLSurface surface;
	 EGLContext context;
	 ANativeWindow* window;

public:
    RenderContextEGL() {};
    virtual ~RenderContextEGL() {};

    /**
     * Initialize an EGL context for the current display.
     */
    virtual bool Create(void* osWnd) override;

    bool HasDisplay();

    /**
     * Tear down the EGL context currently associated with the display.
     */
    virtual void Destroy() override;

    virtual void Resize(int w, int h) override;
    virtual void Swap() override;
    virtual void SetVsync(bool enabled) override;
};

#endif
