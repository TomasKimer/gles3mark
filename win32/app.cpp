

#include "app.h"
#include "console.h"

void App::OnStartup() {
    
    DebugConsole::RedirectIOToConsole();
    
    bool ok = true;
    
    dllHandle = LoadLibrary("gles3mark.dll");
    if (dllHandle != NULL) {

        init    = (dllInit)  GetProcAddress(dllHandle, "DLL_init"  );
        resize  = (dllResize)GetProcAddress(dllHandle, "DLL_resize");
        step    = (dllStep)  GetProcAddress(dllHandle, "DLL_step"  );
        
        if (!init || !resize || !step) {
            ShowMsg("GetProcAddress failed!", MB_ICONERROR);
            FreeLibrary(dllHandle);
            ok = false;
        }        
    }
    else {
        ShowMsg("LoadLibrary failed: gles3mark.dll", MB_ICONERROR);
        ok = false;
    }

    if (!ok) {
        Exit(1);
        return;
    }

    // Create window
    BaseApp::OnStartup();
    
    // Create GL context
    //glContext = new GL3ContextWGL();
    //if (!glContext->Create(GetHWND())) {
    //    delete glContext;
    //    BaseApp::OnQuit();
    //    FreeLibrary(dllHandle);
    //    ShowMsg("Failed to create OpenGL context!", MB_ICONERROR);
    //    Exit(1);
    //    return;
    //}

    init(GetHWND());
    resize(width, height);
}

void App::OnQuit() {
    //glContext->Destroy();
    BaseApp::OnQuit();
    FreeLibrary(dllHandle);
}

void App::OnIdle() {
    step();
    //glContext->Swap();
}

void App::OnResize(int w, int h) {
    BaseApp::OnResize(w, h);
    //if (glContext)
    //    glContext->Resize(true);
    resize(w, h);
}