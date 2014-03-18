

#include "app.h"
#include "console.h"

void App::OnStartup() {

    DebugConsole::RedirectIOToConsole();

    try {
        dll.Init(DLLPATH);
    }
    catch (std::exception e) {
        ShowMsg(e.what(), MB_ICONERROR);
        Exit(1);
        return;
    }
    
    // Create window
    BaseApp::OnStartup();
    
    dll.DllInit(GetHWND());
    dll.DllResize(width, height);
}

void App::OnQuit() {
    BaseApp::OnQuit();
    dll.Free();
}

void App::OnIdle() {
    dll.DllStep();
}

void App::OnResize(int w, int h) {
    BaseApp::OnResize(w, h);
    dll.DllResize(w, h);
}


void App::DLL::Init(const std::string& path) {
    dllHandle = LoadLibrary(path.c_str());

    if (!dllHandle)
        throw std::runtime_error("LoadLibrary failed : " + path);

    DllInit   = (DLLInitT  )GetProcAddress(dllHandle, "DLL_init");
    DllResize = (DLLResizeT)GetProcAddress(dllHandle, "DLL_resize");
    DllStep   = (DLLStepT  )GetProcAddress(dllHandle, "DLL_step");

    if (!DllInit || !DllResize || !DllStep) {
        FreeLibrary(dllHandle);
        throw std::runtime_error("GetProcAddress failed!");
    }
}

void App::DLL::Free() {
    if (dllHandle)
        FreeLibrary(dllHandle);
}
