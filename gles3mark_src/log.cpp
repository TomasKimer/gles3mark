/*
 *  Desc:   Logovani udalosti na std vystup a do souboru,
 *           spolu s casovym razitkem.
 */

#include "log.h"
#include <stdexcept>

#ifdef ANDROID
#include <android/log.h>
#else
#include <iomanip>
#include <ctime>
#include <iostream>
#endif

Log *Log::m_instance = 0;

Log::Log(void)
{
}

Log::~Log(void)
{
    if (m_instance->m_output.is_open())
        m_instance->m_output.close();
}

void Log::Create(const std::string & filename = "app.log")
{
    if (!m_instance)
    {
        m_instance = new Log;
        //m_instance->m_output.open(filename.c_str(), std::ios::out);
        //if (m_instance->m_output.fail())
        //    throw std::runtime_error("Failed to open Log file " + filename + " for writing");
    }
}

void Log::Destroy()
{
    if (m_instance)
    {        
        delete m_instance;
        m_instance = 0;
    }
}

void Log::Msg(const std::string & msg)
{
    Create();
    if (!msg.empty())
        m_instance->logMsg(msg);
}

Log::LogStream Log::Stream()
{
    Create();
    return m_instance->m_stream;
}

void Log::logMsg(const std::string & msg)
{
#ifdef _WIN32
    time_t actTime;
    time(&actTime);
    tm* locTime = localtime(&actTime); //tm locTime; localtime_s(&locTime, &actTime);
    
    std::stringstream timestamp;    
    timestamp << std::setw(2) << std::setfill('0') << locTime->tm_hour
       << ":" << std::setw(2) << std::setfill('0') << locTime->tm_min
       << ":" << std::setw(2) << std::setfill('0') << locTime->tm_sec
       << ": ";    

    std::cout << timestamp.str() << msg << std::endl;    //OutputDebugString(msg.c_str());
#endif

#ifdef ANDROID
    __android_log_print(ANDROID_LOG_DEBUG, "log", "%s", msg.c_str());
#endif

//    m_output << timestamp.str() << msg << std::endl;
//    m_output.flush();
}
