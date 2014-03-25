/*
 *  Desc:   Event logging to stdout/logcat and into a file,
 *           together with a timestamp.
 * TODO
 *  - error param, volba nezapisovat do souboru - automaticky zvolit pry chybe otevreni soubor + informace o tom do logu
 *  - hned msg pri create?
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
class Log
{
public:
    enum class Severity {
        Verbose,
        Debug,
        Info,
        Warn,
        Error
    };    
    
    ~Log(void);

    class LogStream
    {
    public:            
        struct Flush {};  // indicates end of stream
    
        LogStream() {}            
        LogStream(const LogStream& ls)
        {    
            m_cache.str(ls.m_cache.str());
            m_severity = ls.m_severity;
        }
        
        ~LogStream()
        {                
            if (m_cache.tellp() > 0)  // flush on destroy
                m_instance->logMsg(m_cache.str(), m_severity);
        }
    
        template <typename T>
        LogStream& operator<< (const T& v)
        {
            m_cache << v;
            return *this;
        }
    
        LogStream& operator<< (const Flush& v)
        {
            m_instance->logMsg(m_cache.str(), m_severity);
            m_cache.str("");
            return *this;            
        }

        void SetPriority(Severity severity) { m_severity = severity; }
    
    private:
        std::stringstream m_cache;
        Severity m_severity;
    };

    static void      Create (const std::string & filename);
    static void      Destroy();
    static void      Msg(const std::string & msg, Severity severity = Severity::Verbose);
    static LogStream Stream(Severity severity = Severity::Verbose);

private:
    Log(void);
    static Log *m_instance;

    void logMsg(const std::string & msg, Severity severity);

    LogStream m_stream;

    std::ofstream m_output;
};
