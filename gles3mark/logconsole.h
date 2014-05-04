
#pragma once

// console adapter
class LogConsole {
public:
    enum ColorCode {
        BLACK = 0,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHTGRAY,
        DARKGRAY,
        LIGHTBLUE,
        LIGHTGREEN,
        LIGHTCYAN,
        LIGHTRED,
        LIGHTMAGENTA,
        YELLOW,
        WHITE = 15    
    };

    static const int FONT_SIZE = 14;
        
    static const int BUFFER_ROWS = 999; // maximum mumber of lines the output console should have
    static const int VISIBLE_ROWS = 35;
    static const int BUFFER_COLS = 205;
    static const int VISIBLE_COLS = 110; // assert(VISIBLE_COLS <= BUFFER_COLS)

    virtual ~LogConsole() = default;

    virtual void SetAttrib(unsigned short attrib) { }
};

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

/*
 *  http://www.cplusplus.com/forum/windows/58206/
 */
class LogConsoleWin : public LogConsole {

    HANDLE hConsoleOutput;

public:
    LogConsoleWin();
    virtual ~LogConsoleWin();

    void SetSize(SHORT bufferX, SHORT bufferY, SHORT windowX, SHORT windowY);

    // color
    void SetAttrib(unsigned short attrib);

    // Consolas (14), Lucida Console (12), Raster Fonts (X:8,Y:12), 
    void SetFont(WCHAR name[], SHORT sizeY, SHORT sizeX = 0, UINT weight = FW_NORMAL);

    void RedirectIOtoConsole();
    void RedirectToConsole(HANDLE h, const char* mode, FILE* f);
};

#endif
