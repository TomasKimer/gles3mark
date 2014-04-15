/*
 * dllmain.cpp : Defines the entry point for the DLL application.
 *
 *
 */

#ifdef _WIN32

#include <SDKDDKVer.h>        // Defines the highest available Windows platform.
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <memory>

#include "../gles3mark/gles3mark.h"

static std::unique_ptr<GLES3Mark> gles3mark;

void DLL_init(HWND hWnd) {
    gles3mark = std::make_unique<GLES3Mark>();  // gles3mark = std::unique_ptr<GLES3Mark>(new GLES3Mark());
    gles3mark->OnInit((void*)hWnd);
}

void DLL_resize(int width, int height) {
    if (gles3mark)  // !!
        gles3mark->OnResize(width, height);
}

bool DLL_step() {
    return gles3mark->OnStep();
}

void DLL_keyDown(int key) {
    gles3mark->OnKeyDown(Input::OsKeyToKeyCode(key));
}

void DLL_keyUp(int key) {
    gles3mark->OnKeyUp(Input::OsKeyToKeyCode(key));
}

void DLL_mouseMove(int x, int y, int dx, int dy) {
    gles3mark->OnTouchDragged(x, y, dx, dy);
}

/*
 *  Entry point.
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

#endif
