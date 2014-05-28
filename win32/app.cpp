/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include <sstream>

#include "app.h"

void App::OnStartup() {
    try {
        dll.Init(DLLPATH);
    }
    catch (std::exception e) {
        ShowMsg(e.what(), MB_ICONERROR);
        Exit(1);
        return;
    }

    dll.DllPreInit();
    
    // Create window
    BaseApp::OnStartup();
    
    dll.DllInit(GetHWND());
    //dll.DllResize(width, height);
}

void App::OnQuit(int code) {
    BaseApp::OnQuit(code);
    dll.Free();
}

bool App::OnIdle() {
    return dll.DllStep();
}

void App::OnResize(int w, int h) {
    BaseApp::OnResize(w, h);
    dll.DllResize(w, h);
}

void App::OnKeyDown(int key) {
    dll.DllKeyDown(key);
}

void App::OnKeyUp(int key) {
    dll.DllKeyUp(key);
}

void App::OnMouseMove(int x, int y, int dx, int dy) {
    dll.DllMouseMove(x, y, dx, dy);
}

void App::DLL::Init(const std::string& path) {
    dllHandle = LoadLibrary(path.c_str());

    if (!dllHandle) {
        std::stringstream ss;
        ss << GetLastError();
        throw std::runtime_error("LoadLibrary failed : " + path + "\nCode: " + ss.str());
    }        

    DllPreInit   = reinterpret_cast<DLLPreInitT  >(GetProcAddress(dllHandle, "DLL_preInit"  ));
    DllInit      = reinterpret_cast<DLLInitT     >(GetProcAddress(dllHandle, "DLL_init"     ));
    DllResize    = reinterpret_cast<DLLResizeT   >(GetProcAddress(dllHandle, "DLL_resize"   ));
    DllStep      = reinterpret_cast<DLLStepT     >(GetProcAddress(dllHandle, "DLL_step"     ));
    DllKeyDown   = reinterpret_cast<DLLKeyDownT  >(GetProcAddress(dllHandle, "DLL_keyDown"  ));
    DllKeyUp     = reinterpret_cast<DLLKeyUpT    >(GetProcAddress(dllHandle, "DLL_keyUp"    ));
    DllMouseMove = reinterpret_cast<DLLMouseMoveT>(GetProcAddress(dllHandle, "DLL_mouseMove"));

    if (!DllPreInit || !DllInit || !DllResize || !DllStep || !DllKeyDown || !DllKeyUp || !DllMouseMove) {
        FreeLibrary(dllHandle);
        throw std::runtime_error("GetProcAddress failed!");
    }
}

void App::DLL::Free() {
    if (dllHandle)
        FreeLibrary(dllHandle);
}
