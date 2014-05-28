/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

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
#include "iinputlistener.h"
#include "input.h"
#include "assetmanager.h"
#include "scene.h"
#include "time.h"
#include "benchmarkstatistics.h"


/**
 * \brief Lib entry point.
 */
class GLES3Mark : public IGLES3MarkLib, public IInputListener {
    std::unique_ptr<AssetManager> assetManager;
	std::unique_ptr<RenderContextT> glContext;
    std::unique_ptr<Scene> scene;
    Input inputManager;
    Time time;
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

    void OnKeyDown(Input::KeyCode keyCode) override;
    void OnKeyUp(Input::KeyCode keyCode) override;
    void OnTouchDown(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;
    void OnTouchUp(int screenX, int screenY, int pointer = -1, Input::Button button = Input::Button::Default) override;
    void OnTouchDragged(int x, int y, int dx, int dy, int pointer = -1) override;        

private:
    void OnProcessInput();
    void OnDestroy();
};


/**
 * \mainpage Benchmark for OpenGL ES 3.0 Devices
 * 
 * \section intro_sec Description
 *
 * Master's Thesis, FIT BUT Brno 2014
 *
 * <http://gles3mark.appspot.com>
 *
 * <https://play.google.com/store/apps/details?id=com.tomaskimer.gles3mark>
 *
 * <https://github.com/TomasKimer/gles3mark>
 * 
 *
 * \date 2014-05-28
 * \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
 * 
 */