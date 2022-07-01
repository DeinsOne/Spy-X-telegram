#include <spy/service/controller/NewContentController/NewContentController.hpp>
#include <oatpp/core/base/Environment.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/ChatsController/ChatsController.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <algorithm>

#include <spy/service/functions/DownloadFile/DownloadFile.hpp>
#include <spy/utils/StringTools.h>

#include <spy/utils/Logger/SpyLog.h>

#if defined(WIN32) || defined(_WIN32)
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

void spy::service::controller::NewContentController::Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    SPY_LOGD("NewContentController:Initialize");

    auto mePromise = handler->Execute<td::td_api::getMe>();
    auto me = tdlpp::cast_object<td::td_api::user>(mePromise->GetResponse());

    // Get settings controller to operate over excluded chats
    auto settings = this->service->GetController<spy::service::controller::SpySettingsController>();

    auto excludeChats = settings->GetExcludeChats();

    // Add saved messages to excluded chats if wasn't added
    if (std::find(excludeChats.begin(), excludeChats.end(), (int)me.id_) == excludeChats.end()) {
        excludeChats.emplace_back((int)me.id_);
        settings->SetExcludeChats(excludeChats);
    }

    initialized = true;
    SPY_LOGD("NewContentController:Initialize Finished");
}

void spy::service::controller::NewContentController::RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    SPY_LOGD("NewContentController:RegisterUpdates");
    handler->SetCallback<td::td_api::updateNewMessage>(false, [&](td::td_api::updateNewMessage& update) {
        onUpdateNewMessage(update, handler);
    });
    handler->SetCallback<td::td_api::updateFile>(false, [&](td::td_api::updateFile& update) {
        onUpdateFile(update, handler);
    });
    handler->SetCallback<td::td_api::updateChatLastMessage>(false, [&](td::td_api::updateChatLastMessage& update) {
        onUpdateChatLastMessage(update, handler);
    });
}

void spy::service::controller::NewContentController::onUpdateNewMessage(td::td_api::updateNewMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!initialized) return;

    auto settings = this->service->GetController<spy::service::controller::SpySettingsController>();

    // Exclude if chat is a channel
    if (settings->GetForceExcludeChannels() && settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto chats = this->service->GetController<spy::service::controller::ChatsController>();
        auto channels = chats->GetChannelChats();
        if (std::find(channels.begin(), channels.end(), update.message_->chat_id_) != channels.end())
            return;
    }

    // Exclude if chat is not in chat group
    if (settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto excludeChats = settings->GetExcludeChats();
        auto chatIter = std::find(excludeChats.begin(), excludeChats.end(), (int)update.message_->chat_id_);
        if (chatIter != excludeChats.end()) return;
    }
    else {
        auto includeChats = settings->GetIncludeChats();
        auto chatIter = std::find(includeChats.begin(), includeChats.end(), (int)update.message_->chat_id_);
        if (chatIter == includeChats.end()) return;
    }

    // Process message
    td::td_api::downcast_call(*update.message_->sender_id_, tdlpp::overloaded(
        [&](td::td_api::messageSenderChat& sender) {
            SPY_LOGI("NewContentController:onUpdateNewMessage -> chat %d from chat %d", update.message_->chat_id_, sender.chat_id_);
        },
        [&](td::td_api::messageSenderUser& sender) {
            SPY_LOGI("NewContentController:onUpdateNewMessage -> chat %d from user %d", update.message_->chat_id_, sender.user_id_);
        },
        [](auto&) {}
    ));

    // Download file if any is attached
    initiateFileDonwloading(*update.message_->content_, handler);

    // Write message to database
    messagesDb->AddMessage(*update.message_);
}

void spy::service::controller::NewContentController::onUpdateFile(td::td_api::updateFile& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!update.file_->local_->is_downloading_completed_) return;

    // Copy file to local directory
    auto filename = fs::path(update.file_->local_->path_).filename();
    auto newpath = SPY_DATA_FOLDER / fs::path(StringTools::toLower(filename.extension().string().substr(1))) / filename;

    // Skip if this file is already downloaded
    if (fs::is_regular_file(newpath)) return;

    // Create directories for current file parent path
    if (!fs::is_directory(newpath.parent_path())) {
        fs::create_directories(newpath.parent_path());
    }

    // Copy file
    fs::copy(update.file_->local_->path_, newpath);

    // Delete telegram's copy of a file
    handler->Execute<td::td_api::deleteFile>(update.file_->id_);

    // Add file to database
    std::size_t size = 0;
    if (
        update.file_->remote_->is_uploading_completed_ &&
        update.file_->remote_->uploaded_size_ > 0
    ) {
        size = update.file_->remote_->uploaded_size_;
    }
    else if (
        update.file_->local_->is_downloading_completed_ &&
        update.file_->local_->downloaded_size_ > 0
    ) {
        size = update.file_->local_->downloaded_size_;
    }

    messagesDb->AddFile(update.file_->remote_->unique_id_, newpath, size);
}

void spy::service::controller::NewContentController::onUpdateChatLastMessage(td::td_api::updateChatLastMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!initialized) return;

    // Skip if message is empty
    if (!update.last_message_) return;

    chatsDb->SetChatLastMessage((int)update.chat_id_, (int)update.last_message_->id_);
}


void spy::service::controller::NewContentController::initiateFileDonwloading(td::td_api::MessageContent& content, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    td::td_api::downcast_call(content, tdlpp::overloaded(
        [&](td::td_api::messagePhoto& message) {
            auto priority = message.is_secret_ ? 1 : 5;
            spy::service::functions::DownloadFile download(this->service, handler, *message.photo_->sizes_.back()->photo_, priority);
            download.Execute();
        },
        [&](td::td_api::messageVideo& message) {
            auto priority = message.is_secret_ ? 1 : 20;
            spy::service::functions::DownloadFile download(this->service, handler, *message.video_->video_, priority);
            download.Execute();
        },
        [&](td::td_api::messageVoiceNote& message) {
            spy::service::functions::DownloadFile download(this->service, handler, *message.voice_note_->voice_, 10);
            download.Execute();
        },
        [&](td::td_api::messageVideoNote& message) {
            auto priority = message.is_secret_ ? 1 : 4;
            spy::service::functions::DownloadFile download(this->service, handler, *message.video_note_->video_, 10);
            download.Execute();
        },
        [&](td::td_api::messageDocument& message) {
            spy::service::functions::DownloadFile download(this->service, handler, *message.document_->document_, 10);
            download.Execute();
        },
        [&](auto&) {}
    ));
}

