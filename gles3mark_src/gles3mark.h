

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
    Time time;
    FPSCounter fpsCounter;

    bool quit;
    unsigned int score;
    
    // multitouch controls
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
            joystickMove.x = static_cast<float>(joystickMoveCenter.x - x);
            joystickMove.z = static_cast<float>(joystickMoveCenter.y - y);
        }
        else {//if (dx != 0 || dy != 0) {
            gltest->camera.Aim(-dy * 0.002f, -dx * 0.002f);  // 0.0025
        }
    }
    
    void OnResize(int w, int h) override {
        if (glContext)
            glContext->Resize(w, h, false);

        gltest->OnResize(w, h);

        Log::V() << "Resize: " << w << ", " << h;
    }

   
    bool OnStep() override {  // TODO return Exit Code
        if (quit) return false;
        
        time.Update();
        if (time.RealTimeSinceStartup() > 1.5f) {
            fpsCounter.Update(time.DeltaTime());
            if (fpsCounter.JustUpdated())
                Log::V() << time << ", FPS " << fpsCounter;
        }

        OnProcessInput();
        //if (joystickMove.x != 0.f || joystickMove.z != 0.f)
        gltest->camera.Move(joystickMove * time.DeltaTime());


        gltest->OnStep(time);

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        glContext->Swap();

        return true;
    }

    unsigned int GetScore() override {
        return score;
    }


private:
    void OnProcessInput() {
        float step = time.DeltaTime() * 100.0f;

        float x = -((-1.0f * inputManager.IsKeyDown(Input::KeyCode::A)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::D))) * step;
        float z =  ((-1.0f * inputManager.IsKeyDown(Input::KeyCode::S)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::W))) * step;
        
        gltest->camera.Move(glm::vec3(x, 0, z));
    }

    static void doSomeWork(int param, int& outParam) {
        Log::V() << "thread test: hello from thread, param: " << param;
        outParam = 42;
        return;
    }


    // --- TODO? move to frontend main loops? ---
    // http://fabiensanglard.net/timer_and_framerate/index.php#comment-1277535160
    float simulationTime = 0;
    void OnStepFixed() {
        time.Update();
        float realTime = time.RealTimeSinceStartup();
        while (simulationTime < realTime) {
            simulationTime += 0.016f;
            Update(0.016f);
        }
        Render();
    }
    void Update(float deltaTime) {}
    void Render() {}
};
