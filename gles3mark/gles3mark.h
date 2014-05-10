
#pragma once

#ifdef _WIN32
#include <windows.h>
#include "rendercontext_wgl.h"
typedef RenderContextWGL RenderContextT;
#else
#include "rendercontext_egl.h"
typedef RenderContextEGL RenderContextT;
#endif

#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "igles3marklib.h"
#include "input.h"
#include "assetmanager.h"
#include "scene.h"
#include "time.h"
#include "fpscounter.h"
#include "benchmarkstatistics.h"


class GLES3Mark : public IGLES3MarkLib, public IInputListener {
    std::unique_ptr<AssetManager> assetManager;
	std::unique_ptr<RenderContextT> glContext;
    std::unique_ptr<Scene> scene;
    Input inputManager;
    Time time;
    FPSCounter fpsCounter;
    BenchmarkStatistics benchStats;

    bool quit, vsync;
    
    // multitouch controls
    glm::vec3  joystickMove;
    glm::ivec2 joystickMoveCenter;
    int movePointerId, aimPointerId;

public:
    GLES3Mark();
    ~GLES3Mark();

    bool OnInit(void* osWnd, void* ioContext = nullptr) override;
    void OnResize(int w, int h) override;   
    bool OnStep() override;  // TODO return Exit Code
    std::string GetResultJSON() override;

    void OnKeyDown(Input::KeyCode keyCode) override;   // TODO rename to keyPress
    void OnKeyUp(Input::KeyCode keyCode) override;
    void OnTouchDown(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;
    void OnTouchUp(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;
    void OnTouchDragged(int x, int y, int dx, int dy, int pointer = -1) override;        

private:
    void OnProcessInput();
    void OnDestroy();
};
