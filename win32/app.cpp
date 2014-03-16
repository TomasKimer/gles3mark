

#include "app.h"
#include "console.h"

void App::OnStartup() {
    
    DebugConsole::RedirectIOToConsole();
    
    bool ok = true;
    
    DLLHandle = LoadLibrary(DLLPath.c_str());
    if (DLLHandle != nullptr) {

        DLLInit   = (DLLInitT  )GetProcAddress(DLLHandle, "DLL_init"  );
        DLLResize = (DLLResizeT)GetProcAddress(DLLHandle, "DLL_resize");
        DLLStep   = (DLLStepT  )GetProcAddress(DLLHandle, "DLL_step"  );
        
        if (!DLLInit || !DLLResize || !DLLStep) {
            ShowMsg("GetProcAddress failed!", MB_ICONERROR);
            FreeLibrary(DLLHandle);
            ok = false;
        }        
    }
    else {
        ShowMsg("LoadLibrary failed: " + DLLPath, MB_ICONERROR);
        ok = false;
    }

    if (!ok) {
        Exit(1);
        return;
    }

    // Create window
    BaseApp::OnStartup();
    
    DLLInit(GetHWND());
    DLLResize(width, height);
}

void App::OnQuit() {
    BaseApp::OnQuit();
    FreeLibrary(DLLHandle);
}

void App::OnIdle() {
    DLLStep();
}

void App::OnResize(int w, int h) {
    BaseApp::OnResize(w, h);
    DLLResize(w, h);
}