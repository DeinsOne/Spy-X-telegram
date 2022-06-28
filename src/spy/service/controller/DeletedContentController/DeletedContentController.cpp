#include <spy/service/controller/DeletedContentController/DeletedContentController.hpp>
#include <oatpp/core/base/Environment.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/ChatsController/ChatsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <algorithm>

#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

void spy::service::controller::DeletedContentController::Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    initialized = true;
}

void spy::service::controller::DeletedContentController::RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    handler->SetCallback<td::td_api::updateDeleteMessages>(false, [&](td::td_api::updateDeleteMessages& update) {
        onUpdateDeleteMessages(update, handler);
    })->SetCallback<td::td_api::updateMessageContent>(false, [&](td::td_api::updateMessageContent& update) {
        onUpdateMessageContent(update, handler);
    });
}


void spy::service::controller::DeletedContentController::onUpdateDeleteMessages(td::td_api::updateDeleteMessages& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!initialized) return;

    auto settings = this->service->GetController<spy::service::controller::SpySettingsController>();

    // Skip if not process deleted content
    if (!settings->GetSaveDeletedMedia()) return;

    // Exclude if chat is a channel
    if (settings->GetForceExcludeChannels() && settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto chats = this->service->GetController<spy::service::controller::ChatsController>();
        auto channels = chats->GetChannelChats();
        if (std::find(channels.begin(), channels.end(), update.chat_id_) != channels.end())
            return;
    }

    // Exclude if chat is not in chat group
    if (settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto excludeChats = settings->GetExcludeChats();
        auto chatIter = std::find(excludeChats.begin(), excludeChats.end(), (int)update.chat_id_);
        if (chatIter != excludeChats.end()) return;
    }
    else {
        auto includeChats = settings->GetIncludeChats();
        auto chatIter = std::find(includeChats.begin(), includeChats.end(), (int)update.chat_id_);
        if (chatIter == includeChats.end()) return;
    }

    // Write messages modification to db
    messagesDb->AddDeletedModifications(update.message_ids_, (int)update.chat_id_);
}

void spy::service::controller::DeletedContentController::onUpdateMessageContent(td::td_api::updateMessageContent& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    if (!initialized) return;

    auto settings = this->service->GetController<spy::service::controller::SpySettingsController>();

    // Skip if not process deleted content
    if (!settings->GetSaveDeletedMedia()) return;

    // Exclude if chat is a channel
    if (settings->GetForceExcludeChannels() && settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto chats = this->service->GetController<spy::service::controller::ChatsController>();
        auto channels = chats->GetChannelChats();
        if (std::find(channels.begin(), channels.end(), update.chat_id_) != channels.end())
            return;
    }

    // Exclude if chat is not in chat group
    if (settings->GetChatsGroupType() == ChatsGroupType::include_all) {
        auto excludeChats = settings->GetExcludeChats();
        auto chatIter = std::find(excludeChats.begin(), excludeChats.end(), (int)update.chat_id_);
        if (chatIter != excludeChats.end()) return;
    }
    else {
        auto includeChats = settings->GetIncludeChats();
        auto chatIter = std::find(includeChats.begin(), includeChats.end(), (int)update.chat_id_);
        if (chatIter == includeChats.end()) return;
    }

    // Download file if any is attached
    initiateFileDonwloading(*update.new_content_, handler);

    // Write message modification to bd
    messagesDb->AddMessageModification((int)update.message_id_, (int)update.chat_id_, *update.new_content_);
}


void spy::service::controller::DeletedContentController::initiateFileDonwloading(td::td_api::MessageContent& content, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
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

