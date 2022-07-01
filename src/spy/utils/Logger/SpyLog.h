
#ifndef spy_Log
#define spy_Log

#include <spy/utils/Logger/Logger.hpp>

#define SPY_LOG_INIT()      spy::utils::logger::Logger::Get();

#define SPY_LOGD(...)       spy::utils::logger::Logger::Get()->AddLog(plog::debug, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
#define SPY_LOGI(...)       spy::utils::logger::Logger::Get()->AddLog(plog::info, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
#define SPY_LOGW(...)       spy::utils::logger::Logger::Get()->AddLog(plog::warning, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
#define SPY_LOGE(...)       spy::utils::logger::Logger::Get()->AddLog(plog::error, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)
#define SPY_LOGF(...)       spy::utils::logger::Logger::Get()->AddLog(plog::fatal, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), __VA_ARGS__)

#endif // spy_Log
