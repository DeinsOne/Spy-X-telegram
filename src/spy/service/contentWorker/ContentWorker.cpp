
#include <spy/service/contentWorker/ContentWorker.hpp>
#include <spy/service/contentWorker/messageContent/MessageText.hpp>
#include <spy/service/contentWorker/messageContent/MessagePhoto.hpp>
#include <spy/service/contentWorker/messageContent/MessageVideo.hpp>
#include <spy/service/contentWorker/messageContent/MessageVoiceNote.hpp>
#include <spy/service/contentWorker/messageContent/MessageVideoNote.hpp>
#include <spy/service/contentWorker/messageContent/MessageDocument.hpp>

#include <unordered_map>
#include <IdNameBinding.hpp>

void spy::service::content::ContentWorkerFactory::AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) {
    if (contentWorker) {
        SPY_LOGD("ContentWorkerFactory:AddToDatabase -> %s id: %ld", TDLPP_TD_ID_NAME(content.get_id()), messageId);
        contentWorker->AddToDatabase(content, version);
    }
}
void spy::service::content::ContentWorkerFactory::DownloadContent(
    td::td_api::MessageContent& content,
    const std::shared_ptr<controller::ControllersHandler>& controllers,
    const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
) {
    if (contentWorker) {
        SPY_LOGD("ContentWorkerFactory:DownloadContent -> %s id: %ld", TDLPP_TD_ID_NAME(content.get_id()), messageId);
        contentWorker->DownloadContent(content, controllers, handler);
    }
}

oatpp::Any spy::service::content::ContentWorkerFactory::GetFromDatabase(const std::int32_t& version) {
    if (contentWorker) {
        return contentWorker->GetFromDatabase(version);
    }

    return nullptr;
}

spy::service::content::ContentWorkerFactory::ContentWorkerFactory(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id) {
    contentWorker = matchContent(content, message_id, chat_id);
    messageId = (int)message_id;
}

spy::service::content::ContentWorkerFactory::ContentWorkerFactory(const std::int32_t& td_type_id, const std::int64_t& message_id, const std::int64_t& chat_id) {
    contentWorker = matchContent(td_type_id, message_id, chat_id);
    messageId = (int)message_id;
}

std::shared_ptr<spy::service::content::IContentWorker> spy::service::content::ContentWorkerFactory::matchContent(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id) {
    return matchContent(content.get_id(), message_id, chat_id);
}

std::shared_ptr<spy::service::content::IContentWorker> spy::service::content::ContentWorkerFactory::matchContent(const std::int32_t& td_type_id, const std::int64_t& message_id, const std::int64_t& chat_id) {
    switch (td_type_id) {
        case td::td_api::messageText::ID: return std::make_shared<spy::service::content::MessageText>(message_id, chat_id);
        case td::td_api::messagePhoto::ID: return std::make_shared<spy::service::content::MessagePhoto>(message_id, chat_id);
        case td::td_api::messageVideo::ID: return std::make_shared<spy::service::content::MessageVideo>(message_id, chat_id);
        case td::td_api::messageVoiceNote::ID: return std::make_shared<spy::service::content::MessageVoiceNote>(message_id, chat_id);
        case td::td_api::messageVideoNote::ID: return std::make_shared<spy::service::content::MessageVideoNote>(message_id, chat_id);
        case td::td_api::messageDocument::ID: return std::make_shared<spy::service::content::MessageDocument>(message_id, chat_id);
    }

    return nullptr;
}
