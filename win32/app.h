
#pragma once

#include "baseapp.h"

// DLL Imported functions
typedef void(*dllInit)(HWND);
typedef void(*dllResize)(int, int);
typedef void(*dllStep)();

class App : public BaseApp {

    HINSTANCE       dllHandle;
    //GL3ContextWGL*  glContext;

    dllInit         init;
    dllResize       resize;
    dllStep         step;

    virtual void    OnStartup();
    virtual void    OnQuit();
    virtual void    OnIdle();
    virtual void    OnResize(int w, int h);

public:
    App(HINSTANCE hInstance, LPSTR cmdLine) : BaseApp(hInstance, cmdLine) {}
    
};