#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <oatpp/core/base/Environment.hpp>

#include <spy/utils/StringTools.h>

#include <json/json.h>
#include <fstream>

void spy::service::controller::SpySettingsController::Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    std::ifstream settingsFile("spy-settings.json");

    if (settingsFile.is_open()) {  // Load settings from file
        settingsFile.close();
        ReadSettings("spy-settings.json");
    }
    else {  // Create default settings file as wel as spy setting instance
        settingsFile.close();
        WriteSettings("spy-settings.json");
    }

    initialized = true;
}

void spy::service::controller::SpySettingsController::RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    handler->SetCallback<td::td_api::updateNewMessage>(false, [&](td::td_api::updateNewMessage& update) {
        onUpdateNewMessage(update);
    });
}

void spy::service::controller::SpySettingsController::ReadSettings(const std::string& path) {
    Json::Value json;

    // Read content
    std::ifstream settingsFile("spy-settings.json");
    settingsFile >> json;
    settingsFile.close();

    // Read chatsGroupType variable
    if (!json["chatsGroupType"].empty())
        chatsGroupType = (json["chatsGroupType"].asString() == "exclude_all" ? ChatsGroupType::exclude_all : ChatsGroupType::include_all);

    // Read forceExcludeChannels variable
    if (!json["forceExcludeChannels"].empty())
        forceExcludeChannels = json["forceExcludeChannels"].asBool();

    // Read saveDeletedMedia variable
    if (!json["saveDeletedMedia"].empty())
        saveDeletedMedia = json["saveDeletedMedia"].asBool();

    // Read saveSecretContent variable
    if (!json["saveSecretContent"].empty())
        saveSecretContent = json["saveSecretContent"].asBool();

    // Read saveMediaSizeLimit variable
    if (!json["saveMediaSizeLimit"].empty())
        saveMediaSizeLimit = json["saveMediaSizeLimit"].asFloat();

    // Read includeChats variable
    if (!json["includeChats"].empty())
        for (int i = 0; i < json["includeChats"].size(); i++)
            includeChats.emplace_back(json["includeChats"][i].asInt64());

    // Read excludeChats variable
    if (!json["excludeChats"].empty())
        for (int i = 0; i < json["excludeChats"].size(); i++) {
            excludeChats.emplace_back(json["excludeChats"][i].asInt64());
            // printf("  exclude chat: %ld\n", json["excludeChats"][i].asInt64());
        }

}

void spy::service::controller::SpySettingsController::WriteSettings(const std::string& path) {
    Json::Value json;

    // Write chatsGroupType variable
    json["chatsGroupType"] = chatsGroupType == ChatsGroupType::exclude_all ? "exclude_all" : "include_all";

    // Write forceExcludeChannels variable
    json["forceExcludeChannels"] = forceExcludeChannels;

    // Write saveDeletedMedia variable
    json["saveDeletedMedia"] = saveDeletedMedia;

    // Write saveDeletedMedia variable
    json["saveSecretContent"] = saveSecretContent;

    // Write saveMediaSizeLimit variable
    json["saveMediaSizeLimit"] = saveMediaSizeLimit;

    // Write includeChats variable
    for (size_t i = 0; i < includeChats.size(); i++) {
        json["includeChats"][(int)i] = includeChats.at(i);
    }

    // Write includeChats variable
    for (size_t i = 0; i < excludeChats.size(); i++) {
        json["excludeChats"][(int)i] = excludeChats.at(i);
    }

    // Write settings
    Json::StyledWriter writer;
    std::ofstream settingsFile("spy-settings.json");
    settingsFile << writer.write(json);
    settingsFile.close();
}


void spy::service::controller::SpySettingsController::onUpdateNewMessage(td::td_api::updateNewMessage& update) {
    if (!initialized) return;
}

// Getters
spy::service::controller::ChatsGroupType spy::service::controller::SpySettingsController::GetChatsGroupType() { return chatsGroupType; }
std::vector<std::int64_t> spy::service::controller::SpySettingsController::GetIncludeChats() { return includeChats; }
std::vector<std::int64_t> spy::service::controller::SpySettingsController::GetExcludeChats() { return excludeChats; }
bool spy::service::controller::SpySettingsController::GetForceExcludeChannels() { return forceExcludeChannels; }

bool spy::service::controller::SpySettingsController::GetSaveDeletedMedia() { return saveDeletedMedia; }
bool spy::service::controller::SpySettingsController::GetSaveSecretContent() { return saveSecretContent; }
float spy::service::controller::SpySettingsController::GetSaveMediaSizeLimit() { return saveMediaSizeLimit; }

// Setters
void spy::service::controller::SpySettingsController::SetChatsGroupType(const spy::service::controller::ChatsGroupType& type) {
    chatsGroupType = type;
    WriteSettings("spy-settings.json");
}
void spy::service::controller::SpySettingsController::SetIncludeChats(const std::vector<std::int64_t>& chats) {
    includeChats = chats;
    WriteSettings("spy-settings.json");
}
void spy::service::controller::SpySettingsController::SetExcludeChats(const std::vector<std::int64_t>& chats) {
    excludeChats = chats;
    WriteSettings("spy-settings.json");
}
void spy::service::controller::SpySettingsController::SetForceExcludeChannels(const bool& excludeChanels) {
    forceExcludeChannels = excludeChanels;
    WriteSettings("spy-settings.json");
}

void spy::service::controller::SpySettingsController::SetSaveDeletedMedia(const bool& save) {
    saveDeletedMedia = save;
    WriteSettings("spy-settings.json");
}
void spy::service::controller::SpySettingsController::SetSaveSecretContent(const bool& save) {
    saveSecretContent = save;
    WriteSettings("spy-settings.json");
}
void spy::service::controller::SpySettingsController::SetSaveMediaSizeLimit(const float& size) {
    saveMediaSizeLimit = size;
    WriteSettings("spy-settings.json");
}
