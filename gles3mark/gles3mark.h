

#pragma once

#ifdef _WIN32
#include <windows.h>
#include "rendercontext_wgl.h"
typedef RenderContextWGL RenderContextT;
#else
#include "rendercontext_egl.h"
typedef RenderContextEGL RenderContextT;
#endif

#include <string>
#include <stdexcept>
//#include <atomic>
#include <thread>

#include "assetmanager.h"
#include "scene.h"
#include "log.h"
#include "input.h"
#include "time.h"
#include "fpscounter.h"
#include "glquery.h"

#include <random>
#include <glm/gtc/random.hpp>


class IGLES3MarkLib {
public:
    virtual ~IGLES3MarkLib() = default;
    virtual bool OnInit(void* osWnd, void* ioContext = nullptr) = 0;
    virtual void OnResize(int w, int h) = 0;
    virtual bool OnStep() = 0;
    virtual std::string GetResultXML() = 0;
    virtual RenderContextT* GetContext() = 0;  // TODO
};


class GLES3Mark : public IGLES3MarkLib, public IInputListener {

    AssetManager* assetManager;
    Input inputManager;
	RenderContextT* glContext;
    Scene* scene;
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
        score = (int)glm::linearRand(10.f, 1000.f);

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<unsigned> dist(10, 1000);
        score = dist(mt);
    }

    ~GLES3Mark() {
    	if (glContext) {
    		glContext->Destroy();
            //delete glContext;
    		glContext = nullptr;
    	}
    }
    
    RenderContextT* GetContext() override { return glContext; }

    bool OnInit(void* osWnd, void* ioContext = nullptr) override {
    	assetManager = new AssetManager(ioContext);

        // init GL context
        glContext = new RenderContextT();
        glContext->Create(osWnd);

        // display some GL info
        Log::V() << "GL_VENDOR: "                   + GLQuery::VENDOR();
        Log::V() << "GL_RENDERER: "                 + GLQuery::RENDERER();
        Log::V() << "GL_VERSION: "                  + GLQuery::VERSION();
        Log::V() << "GL_SHADING_LANGUAGE_VERSION: " + GLQuery::SHADING_LANGUAGE_VERSION();

        Log::D() << "Max render buffer size: " << GLQuery::MAX_RENDERBUFFER_SIZE() << ", max samples: " << GLQuery::MAX_SAMPLES();
        Log::D() << "Max texture size: " << GLQuery::MAX_TEXTURE_SIZE();
        glm::ivec2 maxDims = GLQuery::MAX_VIEWPORT_DIMS();
        Log::D() << "Max viewport dims: " << maxDims.x << "x" << maxDims.y;
        Log::D() << "Max color attachments: " << GLQuery::MAX_COLOR_ATTACHMENTS();


        scene = new Scene();
        scene->OnInit(assetManager, glContext->GetWidth(), glContext->GetHeight());
       
        
        //Log::Stream() << "C++ ver: " << (long)__cplusplus;

        //std::atomic<bool> ready (false);
        Log::V() << "Concurrent threads supported: " << std::thread::hardware_concurrency();
        int outParam;
        std::thread t(doSomeWork, 5, std::ref(outParam));
        t.join();
        Log::V() << "thread joined, out param: " << outParam;

        OnResize(glContext->GetWidth(), glContext->GetHeight()); 

        return true;
    }


    void OnKeyDown(Input::KeyCode keyCode) override {   // TODO rename to keyPress
        inputManager.RegisterKeyDown(keyCode);
        
        switch (keyCode) {
            case Input::KeyCode::Escape:
                quit = true;
                break;

            case Input::KeyCode::Return:
                scene->camera.Reset();
                break;

            case Input::KeyCode::Space:
                scene->camera.DebugDump();
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
            scene->camera.Aim(-dy * 0.005f, -dx * 0.005f);  // 0.0025
        }
    }
    
    void OnResize(int w, int h) override {
        if (glContext)
            glContext->Resize(w, h, false);

        scene->OnResize(w, h);

        Log::V() << "Resize: " << w << "x" << h;
    }

   
    bool OnStep() override {  // TODO return Exit Code
        if (quit) {
            OnDestroy();
            return false;
        }
        
        time.Update();
        if (time.RealTimeSinceStartup() > 1.5f) {
            fpsCounter.Update(time.DeltaTime());
            if (fpsCounter.JustUpdated())
                Log::V() << "SPF [ms] " << time << " | FPS " << fpsCounter;
        }

        OnProcessInput();
        //if (joystickMove.x != 0.f || joystickMove.z != 0.f)
        scene->camera.Move(joystickMove * time.DeltaTime());

        scene->OnStep(time);

        //std::this_thread::sleep_for(std::chrono::milliseconds(100));

        glContext->Swap();

        return true;
    }

    std::string GetResultXML() override {
        std::stringstream ss;
        ss << score;
        
        return ss.str();
    }


private:
    void OnProcessInput() {
        float step = time.DeltaTime() * 100.0f;

        float x = -((-1.0f * inputManager.IsKeyDown(Input::KeyCode::A)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::D))) * step;
        float z =  ((-1.0f * inputManager.IsKeyDown(Input::KeyCode::S)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::W))) * step;
        
        scene->camera.Move(glm::vec3(x, 0, z));
    }

    void OnDestroy() {
        // vs destructor?
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
