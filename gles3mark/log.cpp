/*
 *  Desc:   Logovani udalosti na std vystup a do souboru,
 *           spolu s casovym razitkem.
 */

#include "log.h"
#include <stdexcept>

#ifdef ANDROID
#include <android/log.h>
static const char* TAG = "gles3mark log";
#else
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

Log* Log::mInstance = nullptr;

Log::~Log(void) {
    mInstance->logMsg("Log destroyed.", Severity::Verbose);
    if (mInstance->mOutput.is_open())
        mInstance->mOutput.close();
}

void Log::Create(const std::string& filename) {
    if (!mInstance) {
        mInstance = new Log();
#ifndef ANDROID
        mInstance->mOutput.open(filename.c_str(), std::ios::out);
        mInstance->logMsg("Log created.", Severity::Verbose);
        if (mInstance->mOutput.fail())
            mInstance->logMsg("Failed to open Log file " + filename + " for writing", Severity::Warn);
#endif
    }
}

void Log::Destroy() {
    if (mInstance) {        
        delete mInstance;
        mInstance = nullptr;
    }
}

void Log::Msg(const std::string & msg, Severity severity) {
    Create();
    if (!msg.empty())
        mInstance->logMsg(msg, severity);
}

Log::LogStream Log::Stream(Severity severity) {
    Create();
    mInstance->mStream.SetSeverity(severity);
    return mInstance->mStream;
}

void Log::logMsg(const std::string & msg, Severity severity) {
#ifndef ANDROID
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream timestamp;
    timestamp << std::put_time(std::localtime(&in_time_t), "%X") << "  ";  // %Y-%m-%d %X

    unsigned short color = LogConsole::WHITE;

    std::string sseverity;
    switch (severity) {
        case Severity::Debug:
            sseverity = "Debug: ";
            color = LogConsole::LIGHTBLUE;
            break;
        case Severity::Info:
            sseverity = "Info: ";
            color = LogConsole::LIGHTGREEN;
            break;
        case Severity::Warn:
            sseverity = "Warning: ";
            color = LogConsole::MAGENTA;
            break;
        case Severity::Error:
            sseverity = "ERROR: ";
            color = LogConsole::LIGHTRED;
            break;
    }

    console.SetAttrib(color);

    std::cerr << timestamp.str() << sseverity << msg << '\n';    //OutputDebugString(msg.c_str());
    if (mOutput.is_open() /*&& severity == Severity::Verbose*/) {
        mOutput << timestamp.str() << sseverity << msg << '\n';  // std::endl ?
        mOutput.flush();
    }

#else
    int iseverity = ANDROID_LOG_VERBOSE;
    switch (severity) {
        case Severity::Debug:
            iseverity = ANDROID_LOG_DEBUG;
        	break;
        case Severity::Info:
            iseverity = ANDROID_LOG_INFO;
    		break;
        case Severity::Warn:
            iseverity = ANDROID_LOG_WARN;
    		break;
        case Severity::Error:
            iseverity = ANDROID_LOG_ERROR;
    		break;
    }

    __android_log_print(iseverity, TAG, "%s", msg.c_str());
#endif
}
