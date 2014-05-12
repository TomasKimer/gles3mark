
#include <stdexcept>
#include <thread>

#include "gles3mark.h"
#include "log.h"
#include "loadingscreen.h"
#include "statsbuilder_json.h"

GLES3Mark::GLES3Mark() : quit(false), vsync(false), movePointerId(-2), aimPointerId(-2) {
#ifndef _DEBUG
    Log::Create(); // console will be below the main window
#endif      

#ifdef ANDROID
    //vsync = true;
#endif
}

GLES3Mark::~GLES3Mark() {
    if (glContext) {
		glContext->Destroy();
	}
}

bool GLES3Mark::OnInit(void* osWnd, void* ioContext) {
	try {
        assetManager = std::unique_ptr<AssetManager>(new AssetManager(ioContext));

        // init GL context
        glContext = std::unique_ptr<RenderContextT>(new RenderContextT());  //std::make_unique<RenderContextT>();
        glContext->Create(osWnd);
        glContext->SetVsync(vsync);

        LoadingScreen ls(*assetManager, glContext->GetWidth(), glContext->GetHeight());
        glContext->Swap();

        // display some GL info
        Log::V() << "GL_VENDOR: "                   + GLQuery::Get<std::string>(GL_VENDOR);
        Log::V() << "GL_RENDERER: "                 + GLQuery::Get<std::string>(GL_RENDERER);
        Log::V() << "GL_VERSION: "                  + GLQuery::Get<std::string>(GL_VERSION);
        Log::V() << "GL_SHADING_LANGUAGE_VERSION: " + GLQuery::Get<std::string>(GL_SHADING_LANGUAGE_VERSION);

        Log::D() << "Max render buffer size: " << GLQuery::Get<GLint>(GL_MAX_RENDERBUFFER_SIZE) << ", max samples: " << GLQuery::Get<GLint>(GL_MAX_SAMPLES);  // min 2048
        Log::D() << "Max texture size: " << GLQuery::Get<GLint>(GL_MAX_TEXTURE_SIZE); // min 2048
        std::vector<GLint> maxDims = GLQuery::Get<GLint>(GL_MAX_VIEWPORT_DIMS, 2);
        Log::D() << "Max viewport dims: " << maxDims[0] << "x" << maxDims[1];
        Log::D() << "Max color attachments: " << GLQuery::Get<GLint>(GL_MAX_COLOR_ATTACHMENTS); // min 4
        Log::D() << "Max vertex attributes: " << GLQuery::Get<GLint>(GL_MAX_VERTEX_ATTRIBS);        


        scene = std::unique_ptr<Scene>(new Scene());
        scene->OnInit(*assetManager, glContext->GetWidth(), glContext->GetHeight());

    } catch (std::exception& e) {
        Log::E() << "Init exception: " << e.what();
        quit = true;
        return false;    
    }

    OnResize(glContext->GetWidth(), glContext->GetHeight()); // TODO

    time.Init();
    benchStats.StartMeasure();

    return true;
}

void GLES3Mark::OnResize(int w, int h) {
    if (glContext) {
        glContext->Resize(w, h);
        scene->OnResize(w, h);            
    }
    Log::V() << "Resize: " << w << "x" << h; // TODO
}

bool GLES3Mark::OnStep() {  // TODO return Exit Code - if !=0, system("pause") / messagebox
    //if (!glContext->HasDisplay()) return;
    if (quit) {
        OnDestroy();
        return false;
    }
    
    time.Update();
    if (time.RealTimeSinceStartup() > 1.0f) {
        benchStats.OnFrame(time.DeltaTime());
        if (benchStats.GetFPSCounter().JustUpdated())
            Log::V() << "SPF [ms]: " << time << " | FPS: " << benchStats.GetFPSCounter();
    }

    OnProcessInput();
    if (joystickMove.x != 0.f || joystickMove.z != 0.f)
        scene->camera.Move(joystickMove * time.DeltaTime());

    if (!scene->OnStep(time))
        quit = true;

    //std::this_thread::sleep_for(std::chrono::milliseconds(180));

    glContext->Swap();

    return true;
}

// vs destructor?
void GLES3Mark::OnDestroy() {
    benchStats.EndMeasure();
    scene.reset();

    Log::D() << "Result JSON: " << GetResultJSON();
}

std::string GLES3Mark::GetResultJSON() {
    JSONStatsBuilder jsb;
    return jsb.BuildBenchStatsInfo(benchStats).BuildGLinfo().Build();
}

void GLES3Mark::OnProcessInput() {
    float step = time.DeltaTime() * 100.0f;

    float x = -((-1.0f * inputManager.IsKeyDown(Input::KeyCode::A)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::D))) * step;
    float z =  ((-1.0f * inputManager.IsKeyDown(Input::KeyCode::S)) + (1.0f * inputManager.IsKeyDown(Input::KeyCode::W))) * step;
    
    if (x != 0.f || z != 0.f)
        scene->camera.Move(glm::vec3(x, 0, z));

    glm::vec3 lightMove;

    if (inputManager.IsKeyDown(Input::KeyCode::Up   )) lightMove.y =  0.1f;
    if (inputManager.IsKeyDown(Input::KeyCode::Down )) lightMove.y = -0.1f;
    if (inputManager.IsKeyDown(Input::KeyCode::Left )) lightMove.z =  0.1f;
    if (inputManager.IsKeyDown(Input::KeyCode::Right)) lightMove.z = -0.1f;

    if (lightMove != glm::vec3(0)) {
        for (std::unique_ptr<Light>& l : scene->lightDatabase) {
            l->Move(lightMove);
        }
    }
}

void GLES3Mark::OnKeyDown(Input::KeyCode keyCode) {   // TODO rename to keyPress
    inputManager.RegisterKeyDown(keyCode);
    
    switch (keyCode) {
        case Input::KeyCode::Escape:
            quit = true;
            break;

        case Input::KeyCode::Return:
            scene->camera.Reset();
            break;

        case Input::KeyCode::Space:
            Log::V() << scene->camera;
            break;

        case Input::KeyCode::Tab:
            scene->freeCamera = !scene->freeCamera;
            break;

        case Input::KeyCode::Q:
            break;
    }
}

void GLES3Mark::OnKeyUp(Input::KeyCode keyCode) {
    inputManager.RegisterKeyUp(keyCode);
}    

void GLES3Mark::OnTouchDown(int screenX, int screenY, int pointer, Input::Button button) {
    if (pointer == -1) return;
    
//#ifdef _DEBUG // NDEBUG
    if (screenX > 0 && screenX < 100 && screenY > 0 && screenY < 100)
        quit = true;

    if (screenX > glContext->GetWidth() - 100 && screenY > 0 && screenY < 100)
        scene->freeCamera = !scene->freeCamera;
//#endif

    if (screenX < glContext->GetWidth() / 2) {
        movePointerId = pointer;
        joystickMoveCenter = glm::ivec2(screenX, screenY);
    }
    else {
        aimPointerId = pointer;
    }
}

void GLES3Mark::OnTouchUp(int screenX, int screenY, int pointer, Input::Button button) {
    if (pointer == movePointerId) {
        joystickMove = glm::vec3(0);
        movePointerId = -1;
    }
    else if (pointer == aimPointerId) {
        aimPointerId = -1;
    }
}

void GLES3Mark::OnTouchDragged(int x, int y, int dx, int dy, int pointer) {
    if (pointer == movePointerId) {
        joystickMove.x = static_cast<float>(joystickMoveCenter.x - x);
        joystickMove.z = static_cast<float>(joystickMoveCenter.y - y);
    }
    else if (dx != 0 || dy != 0) {
        scene->camera.Aim(-dy * 0.005f, -dx * 0.005f);  // 0.0025
    }
}