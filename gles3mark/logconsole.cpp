/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  */

#include "logconsole.h"

#ifdef _WIN32

LogConsoleWin::LogConsoleWin() {
    AllocConsole();  // allocate a console for this app
    
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    SetFont(L"Consolas", FONT_SIZE);
    SetSize(BUFFER_COLS, BUFFER_ROWS, VISIBLE_COLS - 1, VISIBLE_ROWS);
    RedirectIOtoConsole();
}

LogConsoleWin::~LogConsoleWin() {
    FreeConsole();
}

void LogConsoleWin::SetSize(SHORT bufferX, SHORT bufferY, SHORT windowX, SHORT windowY) {
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    GetConsoleScreenBufferInfo(hConsoleOutput, &coninfo);
    //COORD maxWindowSize = GetLargestConsoleWindowSize(hConsoleOutput);
    
    // set the screen buffer to be big enough to let us scroll text
    coninfo.dwSize.X = bufferX;
    coninfo.dwSize.Y = bufferY;
    SetConsoleScreenBufferSize(hConsoleOutput, coninfo.dwSize);
    
    // set the window dimensions
    coninfo.srWindow.Right = windowX;
    coninfo.srWindow.Bottom = windowY;
    SetConsoleWindowInfo(hConsoleOutput, true, &coninfo.srWindow);
}

// color
// http://www.cplusplus.com/forum/beginner/54360/
void LogConsoleWin::SetAttrib(unsigned short attrib) {
    SetConsoleTextAttribute(hConsoleOutput, attrib);    
}

// Consolas (14), Lucida Console (12), Raster Fonts (X:8,Y:12), 
void LogConsoleWin::SetFont(WCHAR name[], SHORT sizeY, SHORT sizeX, UINT weight) {
    CONSOLE_FONT_INFOEX fontInfo = {0};
    
    //GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo)        
    fontInfo.cbSize       = sizeof(CONSOLE_FONT_INFOEX);
    fontInfo.dwFontSize.X = sizeX; //12
    fontInfo.dwFontSize.Y = sizeY;
    fontInfo.FontWeight   = weight;
    wcscpy_s(fontInfo.FaceName, name);  
    
    SetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);    
}

void LogConsoleWin::RedirectIOtoConsole() {
    RedirectToConsole(hConsoleOutput                , "w", stdout); // redirect unbuffered STDOUT to the console
    RedirectToConsole(GetStdHandle(STD_ERROR_HANDLE), "w", stderr); // redirect unbuffered STDERR to the console
    RedirectToConsole(GetStdHandle(STD_INPUT_HANDLE), "r", stdin ); // redirect unbuffered STDIN  to the console
    
    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    std::ios::sync_with_stdio();
}

void LogConsoleWin::RedirectToConsole(HANDLE h, const char* mode, FILE* f) {
    long lStdHandle = (long)h;
    int hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
    FILE *fp = _fdopen(hConHandle, mode);
    *f = *fp;
    setvbuf(f, nullptr, _IONBF, 0);
}

#endif