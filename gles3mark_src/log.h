/*
 *  Desc:   Event logging to stdout/logcat and into a file,
 *           together with a timestamp.
 * TODO
 *  - error param, volba nezapisovat do souboru - automaticky zvolit pry chybe otevreni soubor + informace o tom do logu
 *  - volba min severity pro konzoli/soubor
 *
 *  //#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
	//#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
	//#include <errno.h> ?
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>

/**
 * Event logging.
 */
class Log {
public:
    enum class Severity {
        Verbose,
        Debug,
        Info,
        Warn,
        Error
    };

private:
    class LogStream {
    public:            
        LogStream() {}            
        LogStream(const LogStream& ls) {    
            mCache.str(ls.mCache.str());
            mSeverity = ls.mSeverity;
        }
        
        ~LogStream() {                
            if (mCache.tellp() > 0)  // flush on destroy
                mInstance->logMsg(mCache.str(), mSeverity);
        }

        struct Flush {};  // indicates end of stream
    
        template <typename T>
        LogStream& operator<< (const T& v) {
            mCache << v;
            return *this;
        }
            
        LogStream& operator<< (const Flush& v) {
            mInstance->logMsg(mCache.str(), mSeverity);
            mCache.str("");
            return *this;            
        }

        void SetSeverity(Severity severity) { mSeverity = severity; }
    
    private:
        std::stringstream mCache;
        Severity mSeverity;
    } mStream;

    static Log* mInstance;
    std::ofstream mOutput;
    
    Log() {}
    void logMsg(const std::string & msg, Severity severity);

public:
    ~Log();
    static void       Create (const std::string & filename);
    static void       Destroy();
    static void       Msg(const std::string & msg, Severity severity = Severity::Verbose);
    static LogStream  Stream(Severity severity = Severity::Verbose);

    
    // --- shortcuts ---
    static void       V(const std::string & msg) { Msg(msg, Severity::Verbose); }
    static void       D(const std::string & msg) { Msg(msg, Severity::Debug); }
    static void       I(const std::string & msg) { Msg(msg, Severity::Info); }
    static void       W(const std::string & msg) { Msg(msg, Severity::Warn); }
    static void       E(const std::string & msg) { Msg(msg, Severity::Error); }
       
    static LogStream  V() { return Stream(Severity::Verbose); }
    static LogStream  D() { return Stream(Severity::Debug  ); }
    static LogStream  I() { return Stream(Severity::Info   ); }
    static LogStream  W() { return Stream(Severity::Warn   ); }
    static LogStream  E() { return Stream(Severity::Error  ); }
};
