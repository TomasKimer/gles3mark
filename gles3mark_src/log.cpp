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
#include <iomanip>
#include <ctime>
#include <iostream>
#endif

Log* Log::mInstance = nullptr;

Log::~Log(void) {
    if (mInstance->mOutput.is_open())
        mInstance->mOutput.close();
}

void Log::Create(const std::string & filename = "app.log") {
    if (!mInstance) {
        mInstance = new Log();
#ifndef ANDROID
        mInstance->mOutput.open(filename.c_str(), std::ios::out);
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
    time_t actTime;
    time(&actTime);
    tm* locTime = localtime(&actTime); //tm locTime; localtime_s(&locTime, &actTime);
    
    std::stringstream timestamp;    
    timestamp << std::setw(2) << std::setfill('0') << locTime->tm_hour
        << ":" << std::setw(2) << std::setfill('0') << locTime->tm_min
        << ":" << std::setw(2) << std::setfill('0') << locTime->tm_sec
        << "  ";

    std::string sseverity;
    switch (severity) {
        case Severity::Debug:
            sseverity = "Debug: ";
            break;
        case Severity::Info:
            sseverity = "Info: ";
            break;
        case Severity::Warn:
            sseverity = "Warning: ";
            break;
        case Severity::Error:
            sseverity = "ERROR: ";
            break;
    }

    std::cerr << timestamp.str() << sseverity << msg << '\n';    //OutputDebugString(msg.c_str());
    if (mOutput.is_open()) {
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
