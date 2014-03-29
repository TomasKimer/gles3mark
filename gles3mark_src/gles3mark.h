

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
#include "inputmanager.h"



class GLES3Mark {

    AssetManager* assetManager;
    InputManager inputManager;
	GlContext* glContext;
    GLTest* gltest;
    bool quit;
    unsigned int score;


public:
    GLES3Mark() : glContext(nullptr), quit(false), score(42) {
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

        //Log::Stream() << "C++ ver: " << (long)__cplusplus;
        
        //std::atomic<bool> ready (false);
        std::thread t(doSomeWork);
        t.join();

        return true;
    }

    void OnKeyDown(int osKey) {
        OnKeyDown(InputManager::OsKeyToKeyCode(osKey));
    }

    void OnKeyUp(int osKey) {
        OnKeyUp(InputManager::OsKeyToKeyCode(osKey));
    }

    void OnKeyDown(InputManager::KeyCode keyCode) {   // TODO rename to keyPress
        inputManager.RegisterKeyDown(keyCode);
        
        switch (keyCode) {
            case InputManager::KeyCode::Escape:
                quit = true;
                break;

            case InputManager::KeyCode::Return:
                gltest->camera.Reset();
                break;

            case InputManager::KeyCode::Space:
                gltest->camera.DebugDump();
                break;
        }
    }

    void OnKeyUp(InputManager::KeyCode keyCode) {
        inputManager.RegisterKeyUp(keyCode);
    }    

    void OnMouseMove(int x, int y, int dx, int dy) {
        if (dx != 0 || dy != 0) {
            gltest->camera.Aim(-dy * 0.001f, -dx * 0.001f);
        }
    }

    void OnProcessInput() {
        if (inputManager.IsKeyDown(InputManager::KeyCode::W))
            gltest->camera.Move(glm::vec3(0, 0, 1));
        if (inputManager.IsKeyDown(InputManager::KeyCode::S))
            gltest->camera.Move(glm::vec3(0, 0, -1));
        if (inputManager.IsKeyDown(InputManager::KeyCode::A))
            gltest->camera.Move(glm::vec3(1, 0, 0));
        if (inputManager.IsKeyDown(InputManager::KeyCode::D))
            gltest->camera.Move(glm::vec3(-1, 0, 0));
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

        Log::V() << "Resize: " << w << ", " << h;
    }

    bool OnStep() {  // TODO return Exit Code
        // draw here
        OnProcessInput();

        if (quit)
            return false;

        gltest->OnStep();

        glContext->Swap();

        return true;
    }

    unsigned int GetScore() {
        return score;
    }
};
