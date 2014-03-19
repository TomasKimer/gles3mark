/*
 * Entry point of Windows application.
 *
 */

#include "app.h"
#include <memory>

// Enable dedicated graphics (Nvidia Optimus)
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::unique_ptr<App> app(new App(hInstance, lpCmdLine));
    
    return app->Run();
}
