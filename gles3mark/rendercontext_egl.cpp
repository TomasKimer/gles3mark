#ifdef ANDROID

#include "rendercontext_egl.h"
#include "gl3stub_egl.h"
#include "log.h"

/**
 * Initialize an EGL context for the current display.
 */
bool RenderContextEGL::Create(void* osWnd) {
	window = reinterpret_cast<ANativeWindow*>(osWnd);

//	eglBindAPI(EGL_OPENGL_ES_API);

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);  //eglGetCurrentDisplay()

	EGLint major, minor;
	eglInitialize(display, &major, &minor);
	Log::V() << "EGL Initialized: " << major << "." << minor;

	/*
	 * Here specify the attributes of the desired configuration.
	 * Below, we select an EGLConfig with at least 8 bits per color
	 * component compatible with on-screen windows
	 */
	const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  // EGL_OPENGL_ES3_BIT_KHR - eglext.h (EGL_KHR_create_context extension)
			//EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_BLUE_SIZE   , 8,
			EGL_GREEN_SIZE  , 8,
			EGL_RED_SIZE    , 8,
			EGL_ALPHA_SIZE  , 8,
			EGL_DEPTH_SIZE  , 24,
			EGL_STENCIL_SIZE, 0,
			EGL_SAMPLE_BUFFERS, EGL_FALSE,
			EGL_SAMPLES, 0,
			EGL_NONE };

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(window, 0, 0, format);

	EGLSurface surface = eglCreateWindowSurface(display, config, window, nullptr);

	//eglSurfaceAttrib(display, surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED); // EGL_BUFFER_DESTROYED

	EGLint contextAttrs[] = {
			EGL_CONTEXT_CLIENT_VERSION, 3,
			EGL_NONE };

	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttrs);  // NULL, NULL
	if (context == EGL_NO_CONTEXT)
		return false;

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		return false; //-1
	}

	EGLint w, h;
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	Log::V() << "EGL surface size: " << w << "x" << h;

	EGLint minSwapInterval, maxSwapInterval;
	eglGetConfigAttrib(display, config, EGL_MIN_SWAP_INTERVAL, &minSwapInterval);
	eglGetConfigAttrib(display, config, EGL_MAX_SWAP_INTERVAL, &maxSwapInterval);
	Log::V() << "EGL swap interval - min: " << minSwapInterval << ", max: " << maxSwapInterval;

    if (!gl3stubInit())
        return false; //throw std::runtime_error("GL stub init failed");

    GLint glmajor, glminor;
    glGetIntegerv(GL_MAJOR_VERSION, &glmajor);
    glGetIntegerv(GL_MINOR_VERSION, &glminor);
    if (glmajor < 3 || (glmajor == 3 && glminor < 0))
        Log::E("OpenGL ES 3.0 is not supported!");
    Log::V() << "Max OpenGL ES version supported: " << glmajor << "." << glminor;

	this->display = display;
	this->context = context;
	this->surface = surface;
	this->mWidth = w;
	this->mHeight = h;

	return true; //0
}

bool RenderContextEGL::HasDisplay() {
	return display != nullptr;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void RenderContextEGL::Destroy() {
	if (display != EGL_NO_DISPLAY) {
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT) {
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE) {
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}

void RenderContextEGL::Resize(int w, int h, bool vsync) {
	mWidth = w;
	mHeight = h;

	eglSwapInterval(display, (int)vsync);  	// Turn off vsync - 0
}
void RenderContextEGL::Swap() /*override*/ {
	eglSwapBuffers(display, surface);
}

#endif

