#include <spy/utils/Logger/Logger.hpp>
#include <spy/utils/CmdParserSingleton.hpp>
#include <unordered_map>

#include <spy/utils/Logger/ColorConsoleAppender.hpp>
#include <spy/utils/Logger/LogFormatter.hpp>

spy::utils::logger::Logger* spy::utils::logger::Logger::Get() {
    static Logger* this_ = new Logger();
    return this_;
}

plog::Severity spy::utils::logger::strToSeverity(const std::string& severity) {
    return plog::severityFromString(severity.c_str());
}

spy::utils::logger::Logger::Logger() {
    // Get severity from env variables
    auto severityString = CmdParserSingleton::Get()->GetArgument<std::string>("log_level");
    auto severity = strToSeverity(severityString);

    // Initialize logger
    loggerInstance = std::make_shared<plog::Logger<SPY_PLOG_INSTANCE>>(severity);

    // FIXME: Find a way to delete appender Add appenders
    static plog::ColorConsoleAppender<plog::LogFormatter>* appender = new plog::ColorConsoleAppender<plog::LogFormatter>();
    loggerInstance->addAppender(appender);
}
