/*
 * dllmain.cpp : Defines the entry point for the DLL application.
 *
 *
 */

#ifdef _WIN32

#include <SDKDDKVer.h>        // Defines the highest available Windows platform.
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include "../gles3mark_src/gles3mark.h"

static GLES3Mark* gles3mark = NULL;


void DLL_init(HWND hWnd) {
    if (gles3mark) {
        delete gles3mark;
        gles3mark = NULL;
    }
    
    gles3mark = new GLES3Mark();
    gles3mark->OnInit((void*)hWnd);
}

void DLL_resize(int width, int height) {
    if (gles3mark)
        gles3mark->OnResize(width, height);
}

void DLL_step() {
    if (gles3mark)
        gles3mark->OnStep();
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
