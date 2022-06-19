#pragma once
#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>

namespace plog {
    template<bool useUtcTime>
    class LogFormatterImpl {
    public:
        static util::nstring header() {
            return util::nstring();
        }

        static util::nstring format(const Record& record) {
            tm t;
            useUtcTime ? plog::util::gmtime_s(&t, &record.getTime().time) : plog::util::localtime_s(&t, &record.getTime().time);

            #if !defined(WIN32) && !defined(_WIN32)
                plog::util::nostringstream column1;
                column1 << PLOG_NSTR("[") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm) << PLOG_NSTR("] ");
                column1 << PLOG_NSTR("[") << std::left << severityToString(record.getSeverity()) << PLOG_NSTR("] ");
                column1 << PLOG_NSTR("tid:") << record.getTid();
            #endif

            plog::util::nostringstream raw;
            #if defined(WIN32) || defined(_WIN32)
                raw << PLOG_NSTR("[") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm) << PLOG_NSTR("] ");
                raw << PLOG_NSTR("[") << std::left << severityToString(record.getSeverity()) << PLOG_NSTR("] ");
                raw << PLOG_NSTR("tid:") << record.getTid() << PLOG_NSTR(" | ");
            #else
                raw << std::setw(33) << std::left << std::setfill(PLOG_NSTR(' ')) << column1.str() << PLOG_NSTR(" | ");
            #endif

            raw << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ");
            raw << record.getMessage() << PLOG_NSTR("\n");

            return raw.str();
        }
    };

    class LogFormatter : public LogFormatterImpl<false> {};
    class LogFormatterUtcTime : public LogFormatterImpl<true> {};
}
