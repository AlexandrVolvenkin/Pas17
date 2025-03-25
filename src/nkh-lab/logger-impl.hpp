#ifndef NLAB_LOGGER_IMPL_HPP
#define NLAB_LOGGER_IMPL_HPP

#include "logger-types.hpp"

#include <iostream>
#include <mutex>
#include <sstream>

#include <stdarg.h>
#include <string.h>

#ifdef __linux__
#include <sys/time.h>
#endif

/*
 * Outputs: COUT or CUSTOM: Android Logcat, FILE, ...
 *
 *  - LOG_OUTPUT_COUT   - ON  by default except Android NDK build
 *  - LOG_OUTPUT_LOGCAT - ON  by default for Android (AOSP and NDK) build
 */

#define NDEBUG
//#define LOG_FUNCTION_ENTER_EXIT_THREAD_ID

#ifndef LOG_OUTPUT_LOGCAT_DISABLED
#if defined ANDROID || defined __ANDROID__
#define LOG_OUTPUT_LOGCAT
#include "logger-logcat.hpp"

#define LOG_OUTPUT_CUSTOM(type, msg) logToLogcat<type>(msg)

#if defined ANDROID // NDK
#define LOG_OUTPUT_COUT_DISABLED
#endif

#endif
#endif

#ifndef LOG_OUTPUT_COUT_DISABLED
#define LOG_OUTPUT_COUT
#endif

#ifndef LOG_OUTPUT_CUSTOM
#define LOG_OUTPUT_CUSTOM(type, msg)
#endif

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
#include <thread>
#endif

namespace nlab
{
namespace logger
{

//extern std::mutex gCoutMutex;

inline const char* getFileNameFromPath(const char* fileFullPath);
inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line);
inline std::string getTimeStamp();
inline std::string getTimeStampEnterExit();

template <MsgType type>
constexpr bool isAllowedToLog()
{
#if defined NDEBUG
    if (type == MsgType::Debug || type == MsgType::FuncEntry || type == MsgType::FuncExit)
    {
        return false;
    }

    else
    {
        return true;
    }

#else
    return true;
#endif
};

template <MsgType type>
class Msg
{
public:
    Msg(const char* fileFullPath, const char* functionName, size_t line)
        : pccConstLogFileName("/home/debian/Pas05DebugLog.txt")//Pas05DebugLog//PasNewLog
    {
        mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line)
                     << " ";
        //        if (isAllowedToLog<type>())
//        {
//            mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line)
//                         << " ";
//        }
//        else
//        {
//            mStreamTolog << toStrFileFunctionLine(getFileNameFromPath(fileFullPath), functionName, line)
//                         << " ";
//        }
    };

    // disallow copying
    Msg(const Msg&) = delete;
    Msg& operator=(const Msg&) = delete;

    // disallow moving
    Msg(Msg&&) = delete;
    Msg& operator=(Msg&&) = delete;

    ~Msg()
    {
        std::lock_guard<std::mutex> lock(gCoutMutex);
        if (isAllowedToLog<type>())
        {
#ifdef LOG_OUTPUT_COUT
            {
                std::cout << getMsgTypeName<type>() << getTimeStamp() <<
                mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(type, mStreamTolog.str());
        }

//            std::lock_guard<std::mutex> guard(gCoutMutex);
        std::fstream file;
        file.open(pccConstLogFileName, std::fstream::out | std::fstream::app);
        file << getMsgTypeName<type>() << getTimeStamp() <<
             mStreamTolog.str() << std::endl;
        file.close();
    };

    template <typename T>
    Msg& operator<<(const T& p)
    {
        mStreamTolog << p;
//        if (isAllowedToLog<type>())
//        {
//            mStreamTolog << p;
//        }
//        else
//        {
//            mStreamTolog << p;
//        }

        return *this;
    }

    char *pccConstLogFileName;

private:
    std::stringstream mStreamTolog;
    std::mutex gCoutMutex;
};

class FunctionEnterExit
{
public:
    FunctionEnterExit(const char* fileFullPath, const char* functionName, size_t line)
        : pccConstLogFileName("/home/debian/PasNewLog.txt")
    {
        mStreamTolog << toStrFileFunctionLine(
                         getFileNameFromPath(fileFullPath), functionName, line);

#ifdef LOG_FUNCTION_ENTER_EXIT_THREAD_ID
        mStreamTolog << " ThreadId: " << std::this_thread::get_id();
#endif

        std::lock_guard<std::mutex> lock(gCoutMutex);
        if (isAllowedToLog<MsgType::FuncEntry>())
        {
#ifdef LOG_OUTPUT_COUT
            {
//                std::lock_guard<std::mutex> lock(gCoutMutex);
//                std::cout << getTimeStamp() << " " << getMsgTypeName<MsgType::FuncEntry>() << " "
//                          << mStreamTolog.str() << std::endl;

                std::cout << getMsgTypeName<MsgType::FuncEntry>() << getTimeStampEnterExit() <<
                mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(MsgType::FuncEntry, mStreamTolog.str());
        }

        std::fstream file;
        file.open(pccConstLogFileName, std::fstream::out | std::fstream::app);
        file << getMsgTypeName<MsgType::FuncEntry>() << getTimeStampEnterExit() <<
             mStreamTolog.str() << std::endl;
        file.close();
    };

    // disallow copying
    FunctionEnterExit(const FunctionEnterExit&) = delete;
    FunctionEnterExit& operator=(const FunctionEnterExit&) = delete;

    // disallow moving
    FunctionEnterExit(FunctionEnterExit&&) = delete;
    FunctionEnterExit& operator=(FunctionEnterExit&&) = delete;

    ~FunctionEnterExit()
    {
        std::lock_guard<std::mutex> lock(gCoutMutex);
        if (isAllowedToLog<MsgType::FuncExit>())
        {
#ifdef LOG_OUTPUT_COUT
            {
//                std::lock_guard<std::mutex> lock(gCoutMutex);
//                std::cout << getTimeStamp() << " " << getMsgTypeName<MsgType::FuncExit>() << " "
//                << mStreamTolog.str() << std::endl;

                std::cout << getMsgTypeName<MsgType::FuncExit>() << getTimeStampEnterExit() <<
                mStreamTolog.str() << std::endl;
            }
#endif

            LOG_OUTPUT_CUSTOM(MsgType::FuncExit, mStreamTolog.str());
        }

        std::fstream file;
        file.open(pccConstLogFileName, std::fstream::out | std::fstream::app);
        file << getMsgTypeName<MsgType::FuncExit>() << getTimeStampEnterExit() <<
             mStreamTolog.str() << std::endl;
        file.close();
    };

    char *pccConstLogFileName;
private:
    std::stringstream mStreamTolog;
    std::mutex gCoutMutex;
};

/* To log format string messages
 *
 * Example:
 * logger::logMsg<logger::msgType::Info>(__FUNCTION__, __FILE__, __LINE__, "Test %d %s %c", 888, "str", 'c' );
 */
template <MsgType type>
inline int logMsg(const char* fileFullPath, const char* functionName, size_t line, const char* fmt, ...)
{
    const int LOG_BUFFER_SIZE = 256;

    char buffer[LOG_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, LOG_BUFFER_SIZE - 1, fmt, args);

    Msg<type>(fileFullPath, functionName, line) << buffer;

    va_end(args);

    return 0; // possible error code, now is used in CHECK to avoid unused expression result
}

inline const char* getFileNameFromPath(const char* fileFullPath)
{
    const char* ret = strrchr(fileFullPath, '/');

    if (ret)
        ++ret;
    else
        ret = fileFullPath;

    return ret;
}

inline std::string toStrFileFunctionLine(const char* fileName, const char* functionName, size_t line)
{
    std::stringstream streamTolog;

    streamTolog << fileName << ':' << line << " " << functionName << "()";

    return streamTolog.str();
}

inline std::string getTimeStampLinux()
{
//    char fmt[32];
//    char buf[32];
//    timeval tv;
//    tm* tm;
//
//    gettimeofday(&tv, NULL);
//    tm = localtime(&tv.tv_sec);
//    strftime(fmt, sizeof(fmt), "%d-%m %H:%M:%S.%%03u | ", tm);
//    snprintf(buf, sizeof(buf), fmt, tv.tv_usec / 1000);
//
//    return std::string(buf);



    std::time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);

    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S | ", timeinfo);
    std::string str(buffer);

    return str;
}

inline std::string getTimeStampEnterExit()
{
    char fmt[32];
    char buf[32];
    timeval tv;
    tm* tm;

    gettimeofday(&tv, NULL);
    tm = localtime(&tv.tv_sec);
    strftime(fmt, sizeof(fmt), "%d-%m %H:%M:%S.%%03u | ", tm);
    snprintf(buf, sizeof(buf), fmt, tv.tv_usec / 1000);

    return std::string(buf);
}


inline std::string getTimeStamp()
{
#ifdef __linux__
    return getTimeStampLinux();
#else
    return {};
#endif
}

} // namespace logger
} // namespace nlab

#endif // NLAB_LOGGER_IMPL_HPP
