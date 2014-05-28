/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "app.h"
#include <memory>

/// Enable dedicated graphics (Nvidia Optimus)
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

/// Entry point of Windows application.
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::unique_ptr<App> app(new App(hInstance, lpCmdLine));
    
    return app->Run();
}
