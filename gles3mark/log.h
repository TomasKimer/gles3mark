/**
  *  gles3mark v1.0
  *
  *  \date   2014-05-28
  *  \author Tomas Kimer <xkimer00@stud.fit.vutbr.cz>
  *  \brief  Event logging to stdout/logcat and into a file together with a timestamp.
  */
/*
 * TODO
 *  - error param, volba nezapisovat do souboru - automaticky zvolit pry chybe otevreni soubor + informace o tom do logu
 *  - volba min severity pro konzoli/soubor
 *  - pri \n rozdelit na dalsi logMsg
 *  - #include <errno.h> ?
 */

#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "logconsole.h"

#ifdef _WIN32
typedef LogConsoleWin LogConsoleT;
#else
typedef LogConsole LogConsoleT;
#endif

/**
 * \brief Event logging.
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
    /**
     * \brief Log stream.
     */
    class LogStream {
    public:            
        LogStream() {}            
        LogStream(const LogStream& ls);        
        ~LogStream();

        struct Flush {};  // indicates end of stream
        LogStream& operator<< (const Flush& v);
        
        template <typename T>
        LogStream& operator<< (const T& v) {
            mCache << v;
            return *this;
        }

        void SetSeverity(Severity severity) { mSeverity = severity; }
    
    private:
        std::stringstream mCache;
        Severity mSeverity;
    } mStream;

    Log() {}
    void logMsg(const std::string & msg, Severity severity);
    
    static Log* mInstance;
    std::ofstream mOutput;

    LogConsoleT console;

public:
    ~Log();
    static void       Create(const std::string& filename = "app.log");
    static void       Destroy();
    static void       Msg(const std::string& msg, Severity severity = Severity::Verbose);
    static LogStream  Stream(Severity severity = Severity::Verbose);
    
    // --- shortcuts ---
    static void       V(const std::string& msg) { Msg(msg, Severity::Verbose); }
    static void       D(const std::string& msg) { Msg(msg, Severity::Debug); }
    static void       I(const std::string& msg) { Msg(msg, Severity::Info); }
    static void       W(const std::string& msg) { Msg(msg, Severity::Warn); }
    static void       E(const std::string& msg) { Msg(msg, Severity::Error); }
       
    static LogStream  V() { return Stream(Severity::Verbose); }
    static LogStream  D() { return Stream(Severity::Debug  ); }
    static LogStream  I() { return Stream(Severity::Info   ); }
    static LogStream  W() { return Stream(Severity::Warn   ); }
    static LogStream  E() { return Stream(Severity::Error  ); }
};
