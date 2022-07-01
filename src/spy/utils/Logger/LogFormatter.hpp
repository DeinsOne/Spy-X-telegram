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
                column1 <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_year + 1900 << PLOG_NSTR("-");
                column1 <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-");
                column1 <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");

                column1 << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":");
                column1 << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":");
                column1 << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".");
                column1 << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm) << PLOG_NSTR(" ");

                column1 << record.getTid() << PLOG_NSTR("| ");
            #endif

            plog::util::nostringstream raw;
            #if defined(WIN32) || defined(_WIN32)
                raw <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_year + 1900 << PLOG_NSTR("-");
                raw <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-");
                raw <<  std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");

                raw << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":");
                raw << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":");
                raw << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".");
                raw << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int>(record.getTime().millitm) << PLOG_NSTR(" ");

                raw << record.getTid() << PLOG_NSTR("| ");
            #else
                raw << column1.str();
            #endif

            raw << record.getMessage() << PLOG_NSTR("\n");

            return raw.str();
        }
    };

    class LogFormatter : public LogFormatterImpl<false> {};
    class LogFormatterUtcTime : public LogFormatterImpl<true> {};
}
