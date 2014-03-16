
#pragma once

#include "baseapp.h"

// DLL Imported functions
typedef void(*DLLInitT  )(HWND);
typedef void(*DLLResizeT)(int, int);
typedef void(*DLLStepT  )();

class App : public BaseApp {

    HINSTANCE       DLLHandle;
    std::string     DLLPath;

    DLLInitT        DLLInit;
    DLLResizeT      DLLResize;
    DLLStepT        DLLStep;

    virtual void    OnStartup()             override;
    virtual void    OnQuit()                override;
    virtual void    OnIdle()                override;
    virtual void    OnResize(int w, int h)  override;

public:
    App(HINSTANCE hInstance, LPSTR cmdLine)
        : BaseApp(hInstance, cmdLine), DLLPath("gles3mark.dll") {}    
};