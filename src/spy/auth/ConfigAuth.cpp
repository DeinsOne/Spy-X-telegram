
#include <spy/auth/ConfigAuth.hpp>

std::shared_ptr<spy::auth::ConfigAuth> spy::auth::ConfigAuth::create(const std::string& filename) {
    return std::make_shared<spy::auth::ConfigAuth>(filename);
}

spy::auth::ConfigAuth::ConfigAuth(const std::string& filename) {
    file = std::make_shared<mINI::INIFile>(filename);
    bool empty = !file->read(data);

    if (empty) {
        data["tdapi"]["td_api_hash"] = "";
        data["tdapi"]["td_api_id"] = "";

        data["tdapi_details"]["td_database_directory"] = "tddata";
        data["tdapi_details"]["td_system_language_code"] = "en";
        data["tdapi_details"]["td_device_model"] = "desktop";

        file->generate(data, true);
    }
}


std::string spy::auth::ConfigAuth::GetApiHash() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return data["tdapi"]["td_api_hash"];
}

std::int64_t spy::auth::ConfigAuth::GetApiId() {
    std::lock_guard<std::mutex> lock(dataMutex);
    if (data["tdapi"]["td_api_id"].empty()) return 0;
    return std::stoi(data["tdapi"]["td_api_id"]);
}

std::string spy::auth::ConfigAuth::GetTdDatabaseDir() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return data["tdapi_details"]["td_database_directory"];
}

std::string spy::auth::ConfigAuth::GetTdLanguageCode() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return data["tdapi_details"]["td_system_language_code"];
}

std::string spy::auth::ConfigAuth::GetTdDeviceModel() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return data["tdapi_details"]["td_device_model"];
}



void spy::auth::ConfigAuth::SetApiHash(const std::string& apiHash) {
    std::lock_guard<std::mutex> lock(dataMutex);
    data["tdapi"]["td_api_hash"] = apiHash;
    file->write(data, true);
}

void spy::auth::ConfigAuth::SetApiId(const std::int64_t& apiId) {
    std::lock_guard<std::mutex> lock(dataMutex);
    data["tdapi"]["td_api_id"] = std::to_string(apiId);
    file->write(data, true);
}

void spy::auth::ConfigAuth::SetTdDatabaseDir(const std::string& dir) {
    std::lock_guard<std::mutex> lock(dataMutex);
    data["tdapi_details"]["td_database_directory"] = dir;
    file->write(data, true);
}

void spy::auth::ConfigAuth::SetTdLanguageCode(const std::string& code) {
    std::lock_guard<std::mutex> lock(dataMutex);
    data["tdapi_details"]["td_system_language_code"] = code;
    file->write(data, true);
}

void spy::auth::ConfigAuth::SetTdDeviceModel(const std::string& model) {
    std::lock_guard<std::mutex> lock(dataMutex);
    data["tdapi_details"]["td_device_model"] = model;
    file->write(data, true);
}

