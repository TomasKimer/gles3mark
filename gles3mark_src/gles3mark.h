

#pragma once

#ifdef _WIN32
#include <windows.h>
#include "gl3context_wgl.h"
typedef GL3ContextWGL GlContext;
#else
#include "gl3context_egl.h"
typedef GL3ContextEGL GlContext;
#endif

#include <string>
#include <stdexcept>
//#include <atomic>
#include <thread>

#include "assetmanager.h"
#include "gltest.h"
#include "log.h"



class GLES3Mark {

    AssetManager* assetManager;
	GlContext* glContext;
    GLTest* gltest;


public:
    GLES3Mark() : glContext(nullptr) {
    	gltest = new GLTest();
    }

    ~GLES3Mark() {
    	if (glContext) {
    		glContext->Destroy();
    		glContext = nullptr;
    	}
    }
    
    GlContext* GetGLContext() { return glContext; }

    bool OnInit(void* osWnd, void* ioContext = nullptr) {
    	assetManager = new AssetManager(ioContext);

        // init gl here
        glContext = new GlContext();
        glContext->Create(osWnd);

        gltest->OnInit(assetManager);

        Log::Msg(std::string("GL_VENDOR: ") + (const char*)glGetString(GL_VENDOR));
        Log::Msg(std::string("GL_RENDERER: ") + (const char*)glGetString(GL_RENDERER));
        Log::Msg(std::string("GL_VERSION: ") + (const char*)glGetString(GL_VERSION));
        //Log::Msg(std::string("GL_EXTENSIONS: ") + (const char*)glGetString(GL_EXTENSIONS));

        //Log::Stream() << "C++ ver: " << (long)__cplusplus;


        //std::atomic<bool> ready (false);
        std::thread t(doSomeWork);
        t.join();

        return true;
    }

    static void doSomeWork(void)
    {
    	Log::Msg("thread test: hello from thread...");
        return;
    }

    void OnResize(int w, int h) {
        if (glContext)
            glContext->Resize(w, h, true);

        gltest->OnResize(w, h);

        Log::Stream() << "Resize: " << w << ", " << h;
    }

    void OnStep() {
        // draw here

        gltest->OnStep();

        glContext->Swap();
    }
};
