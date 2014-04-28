
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
#include <random>
#include <glm/gtc/random.hpp>

#include "assetmanager.h"
#include "scene.h"
#include "log.h"
#include "input.h"
#include "time.h"
#include "fpscounter.h"
#include "glquery.h"
#include "benchmarkstatistics.h"
#include "statsbuilder_json.h"


class IGLES3MarkLib {
public:
    virtual ~IGLES3MarkLib() = default;
    virtual bool OnInit(void* osWnd, void* ioContext = nullptr) = 0;
    virtual void OnResize(int w, int h) = 0;
    virtual bool OnStep() = 0;
    virtual std::string GetResultXML() = 0;
    virtual const RenderContextT* GetContext() = 0;  // TODO
};


class GLES3Mark : public IGLES3MarkLib, public IInputListener {

    AssetManager* assetManager;
    Input inputManager;
	RenderContextT* glContext;
    Scene* scene;
    Time time;
    FPSCounter fpsCounter;
    BenchmarkStatistics benchStats;

    bool quit, vsync;
    
    // multitouch controls
    glm::vec3 joystickMove;
    glm::ivec2 joystickMoveCenter;
    int movePointerId, aimPointerId;


public:
    GLES3Mark() : glContext(nullptr), quit(false), vsync(false), movePointerId(-2), aimPointerId(-2) {
        //Log::Create();  // TODO
    }

    ~GLES3Mark() {
    	if (glContext) {
    		glContext->Destroy();
            //delete glContext;
    		glContext = nullptr;
    	}
    }
    
    const RenderContextT* GetContext() override { return glContext; }

    bool OnInit(void* osWnd, void* ioContext = nullptr) override;

    void OnKeyDown(Input::KeyCode keyCode) override;   // TODO rename to keyPress

    void OnKeyUp(Input::KeyCode keyCode) override;  

    void OnTouchDown(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;

    void OnTouchUp(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;

    void OnTouchDragged(int x, int y, int dx, int dy, int pointer = -1) override;
    
    void OnResize(int w, int h) override;
   
    bool OnStep() override;  // TODO return Exit Code


    std::string GetResultXML() override {
        JSONStatsBuilder jsb;
        return jsb.BuildBenchStatsInfo(benchStats).BuildGLinfo().Build();  //jsb.GetResultJSON(benchStats);
    }


private:
    void OnProcessInput();

    void OnDestroy();




    static void doSomeWork(int param, int& outParam) {
        Log::V() << "thread test: hello from thread, param: " << param;
        outParam = 42;
        return;
    }

    static unsigned randTest() {        
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<unsigned> dist(10, 1000);
        return dist(mt);  // (unsigned)glm::linearRand(10.f, 1000.f);
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
