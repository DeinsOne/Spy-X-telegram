#include <spy/service/controller/ChatsController/ChatsController.hpp>
#include <oatpp/core/base/Environment.hpp>

#include <algorithm>

void spy::service::controller::ChatsController::Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    OATPP_LOGD("ChatsController", "Initialize");

    auto chatsPromise = handler->Execute<td::td_api::getChats>(td::td_api::make_object<td::td_api::chatListMain>(), 1000);
    auto chats = tdlpp::cast_object<td::td_api::chats>(chatsPromise->GetResponse());

    std::condition_variable condition;
    std::int64_t loadedChats = 0;

    // Load all chats
    for (auto chatId : chats.chat_ids_) {
        auto func = td::td_api::make_object<td::td_api::getChat>(chatId);
        handler->ExecuteAsync<td::td_api::getChat>(std::move(func), [&](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
                [&](td::td_api::chat& chat) {
                    InsertChat(chat);
                },
                [](auto&) {}
            ));

            loadedChats++;
            condition.notify_all();
        });
    }

    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    condition.wait(lock, [&]() -> bool { return loadedChats == chats.total_count_; } );

    initialized = true;


    // OATPP_LOGD("ChatsController", "Channels: %d", channelChats.size());
    OATPP_LOGD("ChatsController", "Initialization finished. Fetched %d chats", chats.total_count_);
}

void spy::service::controller::ChatsController::RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    handler->SetCallback<td::td_api::updateNewChat>(false, [&](td::td_api::updateNewChat& update) {
        InsertChat(*update.chat_);
    });
}


void spy::service::controller::ChatsController::InsertChat(td::td_api::chat& origin) {
    if (std::find(allChats.begin(), allChats.end(), origin.id_) == allChats.end()) {
        allChats.emplace_back(origin.id_);
    }

    td::td_api::downcast_call(*origin.type_, tdlpp::overloaded(
        [&](td::td_api::chatTypeBasicGroup& chat) {
            if (std::find(basicGroupsChats.begin(), basicGroupsChats.end(), origin.id_) == basicGroupsChats.end()) {
                basicGroupsChats.emplace_back(origin.id_);
            }
        },
        [&](td::td_api::chatTypePrivate& chat) {
            if (std::find(privateChats.begin(), privateChats.end(), origin.id_) == privateChats.end()) {
                privateChats.emplace_back(origin.id_);
            }
        },
        [&](td::td_api::chatTypeSecret& chat) {
            if (std::find(secretChats.begin(), secretChats.end(), origin.id_) == secretChats.end()) {
                secretChats.emplace_back(origin.id_);
            }
        },
        [&](td::td_api::chatTypeSupergroup& chat) {
            if (std::find(supergroupChats.begin(), supergroupChats.end(), origin.id_) == supergroupChats.end()) {
                supergroupChats.emplace_back(origin.id_);
            }

            if (chat.is_channel_) {
                if (std::find(channelChats.begin(), channelChats.end(), origin.id_) == channelChats.end()) {
                    channelChats.emplace_back(origin.id_);
                }
            }
        },
        [](auto&) { }
    ));

    auto chatDto = dto::Chat::createShared();
    chatDto->id = (int)origin.id_;
    chatDto->title = origin.title_;
    chatDto->fetchSince = StringTools::currentDatTime();
    chatDto->fetched = chatDto->fetchSince;

    if (origin.last_message_) {
        chatDto->lastMessageId = (int)origin.last_message_->id_;
    }

    // Try to add chat to chats database
    chatsDb->addChat(chatDto);
}



std::vector<std::int64_t> spy::service::controller::ChatsController::GetAllChats() {
    return allChats;
}
std::vector<std::int64_t> spy::service::controller::ChatsController::GetBasicGroupsChats() {
    return basicGroupsChats;
}
std::vector<std::int64_t> spy::service::controller::ChatsController::GetPrivateChats() {
    return privateChats;
}
std::vector<std::int64_t> spy::service::controller::ChatsController::GetSecretChats() {
    return secretChats;
}
std::vector<std::int64_t> spy::service::controller::ChatsController::GetSupergroupChats() {
    return supergroupChats;
}
std::vector<std::int64_t> spy::service::controller::ChatsController::GetChannelChats() {
    return channelChats;
}

