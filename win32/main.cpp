/*
 * Entry point of Windows application.
 *
 */

#include "app.h"

// Enable dedicated graphics (Nvidia Optimus)
extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    App *app = new App(hInstance, lpCmdLine);

    int ret = app->Run();

    delete app;

    return ret;
}
