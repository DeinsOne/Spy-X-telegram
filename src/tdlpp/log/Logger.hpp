
#ifndef tdlpp_Logger
#define tdlpp_Logger

#include <IdNameBinding.hpp>

#if defined(TDLPP_ENABLE_OBJECT_LOGGING) || defined(TDLPP_ENABLE_FILE_LOGGING) || defined(TDLPP_ENABLE_CONSOLE_LOGGING)
    #define __TDLPP_LOGGING_ENABLE
#endif

#if defined(__TDLPP_LOGGING_ENABLE)
    #include <plog/Log.h>
    #include <plog/Init.h>

    #include <plog/Appenders/ColorConsoleAppender.h>
    #include <plog/Appenders/RollingFileAppender.h>
    #include <tdlpp/log/LogFormatter.h>

    #include <plog/Record.h>
    #include <plog/Util.h>
    #include <iomanip>
#endif

#if defined(__TDLPP_LOGGING_ENABLE)
namespace tdlpp { namespace log {

    class Logger {
    public:
        static Logger* Get() {
            static Logger* this_ = new Logger();
            return this_;
        }

        template<typename... TArgs>
        void AddLog(const plog::Severity& level, const std::string& func, const std::size_t& line, const std::string& file, TArgs... args) {
            IF_PLOG_(0, level) (*plog::get<0>()) += plog::Record(level, func.c_str(), line, file.c_str(), PLOG_GET_THIS(), 0).ref().printf(args...);
            IF_PLOG_(1, level) (*plog::get<1>()) += plog::Record(level, func.c_str(), line, file.c_str(), PLOG_GET_THIS(), 1).ref().printf(args...);
        }

        template<typename... TArgs>
        void AddObjectLog(TArgs... args) {
            PLOG_(2, plog::Severity::verbose).printf(args...);
        }

    private:
        Logger() {
            #ifdef TDLPP_ENABLE_CONSOLE_LOGGING
                consoleAppender = std::make_shared<plog::ColorConsoleAppender<plog::LogFormatter>>();
                plog::init<0>((plog::Severity)TDLPP_CONSOLE_LOGGING_LEVEL, consoleAppender.get());
            #endif

            #ifdef TDLPP_ENABLE_FILE_LOGGING
                rollingFileAppender = std::make_shared<plog::RollingFileAppender<plog::LogFormatter>>("tdlpp.log", TDLPP_FILE_LOGGING_MAX_FILE_SIZE, TDLPP_FILE_LOGGING_MAX_FILES);
                plog::init<1>((plog::Severity)TDLPP_FILE_LOGGING_LEVEL, rollingFileAppender.get());
                // plog::get<0>()->addAppender(rollingFileAppender.get());
            #endif

            #ifdef TDLPP_ENABLE_OBJECT_LOGGING
                objectFileAppender = std::make_shared<plog::RollingFileAppender<plog::LogFormatter>>("objects.log", TDLPP_OBJECT_LOGGING_MAX_FILE_SIZE, TDLPP_OBJECT_LOGGING_MAX_FILES);
                plog::init<2>((plog::Severity)plog::Severity::verbose, objectFileAppender.get());
            #endif
        }

        #ifdef TDLPP_ENABLE_CONSOLE_LOGGING
            std::shared_ptr<plog::ConsoleAppender<plog::LogFormatter>> consoleAppender;
        #endif

        #ifdef TDLPP_ENABLE_FILE_LOGGING
            std::shared_ptr<plog::RollingFileAppender<plog::LogFormatter>> rollingFileAppender;
        #endif

        #ifdef TDLPP_ENABLE_OBJECT_LOGGING
            std::shared_ptr<plog::RollingFileAppender<plog::LogFormatter>> objectFileAppender;
        #endif
    };

} // namespace log
} // namespace tdlpp
#endif // __TDLPP_LOGGING_ENABLE

#define TDLPP_TD_ID_NAME(x)             tdlpp::log::__IdNameBinding(x).c_str()


#if !defined(__TDLPP_LOGGING_ENABLE)
    #define TDLPP_LOG_VERBOSE(...)
    #define TDLPP_LOG_DEBUG(...)
    #define TDLPP_LOG_INFO(...)
    #define TDLPP_LOG_WARNING(...)
    #define TDLPP_LOG_ERROR(...)
    #define TDLPP_LOG_FATAL(...)
    #define TDLPP_LOG_NONE(...)

    #define TDLPP_OBJECT_LOG(...)
#else
    #define TDLPP_LOG_VERBOSE(...)      tdlpp::log::Logger::Get()->AddLog(plog::verbose, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_DEBUG(...)        tdlpp::log::Logger::Get()->AddLog(plog::debug, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_INFO(...)         tdlpp::log::Logger::Get()->AddLog(plog::info, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_WARNING(...)      tdlpp::log::Logger::Get()->AddLog(plog::warning, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_ERROR(...)        tdlpp::log::Logger::Get()->AddLog(plog::error, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_FATAL(...)        tdlpp::log::Logger::Get()->AddLog(plog::fatal, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
    #define TDLPP_LOG_NONE(...)         tdlpp::log::Logger::Get()->AddLog(plog::none, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)

    #define TDLPP_OBJECT_LOG(...)       tdlpp::log::Logger::Get()->AddObjectLog(__VA_ARGS__)
#endif // __TDLPP_LOGGING_ENABLE



#endif // tdlpp_Logger
