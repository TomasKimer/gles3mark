

#pragma once

#ifdef _WIN32
#include <windows.h>
#include "gl3context_wgl.h"
typedef GL3ContextWGL GlContext;
#else
#include "gles3context_egl.h"
typedef GLES3ContextEGL GlContext;
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

    	int bytesRead = 0;
    	int bytesToRead = 1023;
    	char buffer[1024];
    	bytesRead = assetManager->ReadAsset("testasset.txt", 1023, buffer);
        Log::Stream() << "bytes read: " << bytesRead;
    	std::string msg(buffer, bytesRead);
    	Log::Msg(msg);

        // init gl here
        glContext = new GlContext();
        glContext->Create(osWnd);

        gltest->OnInit();

        Log::Msg((const char*)glGetString(GL_VENDOR));
        Log::Msg((const char*)glGetString(GL_RENDERER));
        Log::Msg((const char*)glGetString(GL_VERSION));
        //Log::Msg((const char*)glGetString(GL_EXTENSIONS));

        Log::Stream() << "C++ ver: " << (long)__cplusplus;

        
        try {
        	throw std::runtime_error("blah");
        } catch (std::exception &e) {
            Log::Msg(std::string("exception test: ") + e.what());
        }

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
