#include <spy/service/controller/NewContentController/NewContentController.hpp>
#include <oatpp/core/base/Environment.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/ChatsController/ChatsController.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <algorithm>

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <spy/utils/StringTools.h>

#include <spy/utils/Logger/SpyLog.h>
#include <IdNameBinding.hpp>

#if (CXX_FILESYSTEM_IS_EXPERIMENTAL == true)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #include <filesystem>
    namespace fs = std::filesystem;
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
    SPY_LOGD("NewContentController:onUpdateNewMessage -> %s id: %ld", TDLPP_TD_ID_NAME(update.message_->content_->get_id()), update.message_->id_);

    // Download file if any is attached
    auto worker = std::make_shared<service::content::ContentWorkerFactory>(*update.message_->content_, (int)update.message_->id_, (int)update.message_->chat_id_);
    worker->DownloadContent(*update.message_->content_, this->service, handler);

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

    messagesDb->AddFile(update.file_->remote_->unique_id_, newpath.string(), size);
}

void spy::service::controller::NewContentController::onUpdateChatLastMessage(td::td_api::updateChatLastMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!initialized) return;

    // Skip if message is empty
    if (!update.last_message_) return;

    chatsDb->SetChatLastMessage((int)update.chat_id_, (int)update.last_message_->id_);
}
