
#pragma once

#include "baseapp.h"

#include <string>
#include <stdexcept>

const std::string DLLPATH("gles3mark.dll");

class App : public BaseApp {

    class DLL {
        // DLL Imported functions
        using DLLInitT   = void(*)(HWND    );   // typedef void(*DLLInitT  )(HWND    );
        using DLLResizeT = void(*)(int, int);   // typedef void(*DLLResizeT)(int, int);
        using DLLStepT   = void(*)(        );   // typedef void(*DLLStepT  )(        );

        HINSTANCE   dllHandle;
    public:
        DLLInitT    DllInit;
        DLLResizeT  DllResize;
        DLLStepT    DllStep;

        DLL() : dllHandle(nullptr) {}
        void Init(const std::string& path);
        void Free();
    } dll;

    virtual void    OnStartup()             override;
    virtual void    OnQuit()                override;
    virtual void    OnIdle()                override;
    virtual void    OnResize(int w, int h)  override;

public:
    App(HINSTANCE hInstance, LPSTR cmdLine)
        : BaseApp(hInstance, cmdLine) {}    
};