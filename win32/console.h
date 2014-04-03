
#pragma once

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

/*
 *  http://www.cplusplus.com/forum/windows/58206/
 */
class DebugConsole {

    // maximum mumber of lines the output console should have
    static const int BUFFER_ROWS = 999;
    static const int VISIBLE_ROWS = 35;

    static const int BUFFER_COLS = 205;
    static const int VISIBLE_COLS = 100; // assert(VISIBLE_COLS <= BUFFER_COLS)

public:

    static void RedirectIOToConsole() {
        // allocate a console for this app
        AllocConsole();

        HANDLE hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        //COORD maxWindowSize = GetLargestConsoleWindowSize(hOutHandle);

        CONSOLE_SCREEN_BUFFER_INFO coninfo;
        GetConsoleScreenBufferInfo(hOutHandle, &coninfo);
        
        // set the screen buffer to be big enough to let us scroll text
        coninfo.dwSize.Y = BUFFER_ROWS;
        coninfo.dwSize.X = BUFFER_COLS;
        SetConsoleScreenBufferSize(hOutHandle, coninfo.dwSize);
        
        // set the window dimensions
        coninfo.srWindow.Right = VISIBLE_COLS - 1;
        coninfo.srWindow.Bottom = VISIBLE_ROWS;
        SetConsoleWindowInfo(hOutHandle, true, &coninfo.srWindow);

        // redirect unbuffered STDOUT to the console
        long lStdHandle = (long)hOutHandle;
        int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        FILE *fp = _fdopen(hConHandle, "w");
        *stdout = *fp;
        setvbuf(stdout, nullptr, _IONBF, 0);

        // redirect unbuffered STDIN to the console
        lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        fp = _fdopen(hConHandle, "r");
        *stdin = *fp;
        setvbuf(stdin, nullptr, _IONBF, 0);

        // redirect unbuffered STDERR to the console
        lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

        fp = _fdopen(hConHandle, "w");
        *stderr = *fp;
        setvbuf(stderr, nullptr, _IONBF, 0);

        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
        std::ios::sync_with_stdio();
    }
};
