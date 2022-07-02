
#ifndef spy_Logger
#define spy_Logger

#include <plog/Log.h>
#include <memory>

#define SPY_PLOG_INSTANCE 890

namespace spy { namespace utils { namespace logger {

    plog::Severity strToSeverity(const std::string& severity);

    class Logger {
    public:
        static Logger* Get();

        template<typename... TArgs>
        void AddLog(const plog::Severity& level, const std::string& func, const std::size_t& line, const std::string& file, TArgs... args) {
            IF_PLOG_(SPY_PLOG_INSTANCE, level) (*plog::get<SPY_PLOG_INSTANCE>()) += 
                plog::Record(
                    level,
                    func.c_str(),
                    line, file.c_str(),
                    PLOG_GET_THIS(),
                    SPY_PLOG_INSTANCE
                )
                .ref()
                .printf(args...);
        }

    private:
        Logger();

        std::shared_ptr<plog::Logger<SPY_PLOG_INSTANCE>> loggerInstance;
    };

} } } // namespace spy { namespace utils { namespace logger {

#endif // spy_Logger
