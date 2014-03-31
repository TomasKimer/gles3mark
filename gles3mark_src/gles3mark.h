

#pragma once

#ifdef _WIN32
#include <windows.h>
#include "glcontext_wgl.h"
typedef GLContextWGL GlContext;
#else
#include "glcontext_egl.h"
typedef GLContextEGL GlContext;
#endif

#include <string>
#include <stdexcept>
//#include <atomic>
#include <thread>

#include "assetmanager.h"
#include "gltest.h"
#include "log.h"
#include "input.h"
#include "time.h"
#include "fpscounter.h"


class IGLES3MarkLib {
public:
    virtual ~IGLES3MarkLib() = default;
    virtual bool OnInit(void* osWnd, void* ioContext = nullptr) = 0;
    virtual void OnResize(int w, int h) = 0;
    virtual bool OnStep() = 0;
    virtual unsigned int GetScore() = 0;
    virtual GlContext* GetGLContext() = 0;  // TODO
};


class GLES3Mark : public IGLES3MarkLib, public IInputListener {

    AssetManager* assetManager;
    Input inputManager;
	GlContext* glContext;
    GLTest* gltest;
    bool quit;
    unsigned int score;
    
    glm::vec3 joystickMove;
    glm::ivec2 joystickMoveCenter;
    int movePointerId, aimPointerId;


public:
    GLES3Mark() : glContext(nullptr), quit(false), score(42), movePointerId(-2), aimPointerId(-2) {
    	gltest = new GLTest();
    }

    ~GLES3Mark() {
    	if (glContext) {
    		glContext->Destroy();
    		glContext = nullptr;
    	}
    }
    
    GlContext* GetGLContext() override { return glContext; }

    bool OnInit(void* osWnd, void* ioContext = nullptr) override {
    	assetManager = new AssetManager(ioContext);

        // init gl here
        glContext = new GlContext();
        glContext->Create(osWnd);

        gltest->OnInit(assetManager);

        //Log::Stream() << "C++ ver: " << (long)__cplusplus;
        
        
        StopWatch<std::chrono::high_resolution_clock> sw;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Log::V() << "Elapsed time: " << sw.elapsed<std::chrono::nanoseconds>() << " nanoseconds";
        
        //std::atomic<bool> ready (false);
        Log::V() << "Concurrent threads supported: " << std::thread::hardware_concurrency();
        int outParam;
        std::thread t(doSomeWork, 5, std::ref(outParam));
        t.join();
        Log::V() << "thread joined, out param: " << outParam;

        return true;
    }


    void OnKeyDown(Input::KeyCode keyCode) override {   // TODO rename to keyPress
        inputManager.RegisterKeyDown(keyCode);
        
        switch (keyCode) {
            case Input::KeyCode::Escape:
                quit = true;
                break;

            case Input::KeyCode::Return:
                gltest->camera.Reset();
                break;

            case Input::KeyCode::Space:
                gltest->camera.DebugDump();
                break;
        }
    }

    void OnKeyUp(Input::KeyCode keyCode) override {
        inputManager.RegisterKeyUp(keyCode);
    }    

    void OnTouchDown(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override {
        if (pointer == -1) return;
        
        if (screenX > 0 && screenX < 100 && screenY > 0 && screenY < 100)
            quit = true;

        if (screenX < glContext->GetWidth() / 2) {
            movePointerId = pointer;
            joystickMoveCenter = glm::ivec2(screenX, screenY);
        }
        else {
            aimPointerId = pointer;
        }
    }

    void OnTouchUp(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override {
        if (pointer == movePointerId) {
            joystickMove = glm::vec3(0);
            movePointerId = -1;
        }
        else if (pointer == aimPointerId) {
            aimPointerId = -1;
        }
    }

    void OnTouchDragged(int x, int y, int dx, int dy, int pointer = -1) override {
        if (pointer == movePointerId) {
            joystickMove.x = (joystickMoveCenter.x - x) * 0.01f;
            joystickMove.z = (joystickMoveCenter.y - y) * 0.01f;
        }
        else if (dx != 0 || dy != 0) {
            gltest->camera.Aim(-dy * 0.001f, -dx * 0.001f);
        }
    }
    
    void OnResize(int w, int h) override {
        if (glContext)
            glContext->Resize(w, h, true);

        gltest->OnResize(w, h);

        Log::V() << "Resize: " << w << ", " << h;
    }

    bool OnStep() override {  // TODO return Exit Code
        // draw here
        OnProcessInput();
        if (joystickMove.x != 0.f || joystickMove.z != 0.f)
            gltest->camera.Move(joystickMove);

        if (quit)
            return false;

        gltest->OnStep();

        glContext->Swap();

        return true;
    }

    unsigned int GetScore() override {
        return score;
    }


private:
    void OnProcessInput() {
        if (inputManager.IsKeyDown(Input::KeyCode::W))
            gltest->camera.Move(glm::vec3(0, 0, 1));
        if (inputManager.IsKeyDown(Input::KeyCode::S))
            gltest->camera.Move(glm::vec3(0, 0, -1));
        if (inputManager.IsKeyDown(Input::KeyCode::A))
            gltest->camera.Move(glm::vec3(1, 0, 0));
        if (inputManager.IsKeyDown(Input::KeyCode::D))
            gltest->camera.Move(glm::vec3(-1, 0, 0));
    }

    static void doSomeWork(int param, int& outParam) {
        Log::V() << "thread test: hello from thread, param: " << param;
        outParam = 42;
        return;
    }
};
