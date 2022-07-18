
#include <spy/service/controller/DeletedContentChatController/Command/HelpCommand/HelpCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>
#include <spy/service/controller/SettingsController/SpySettingsController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>


spy::service::controller::command::HelpCommand::HelpCommand(
    const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
    const std::shared_ptr<ControllersHandler>& controllerHandler
) : tdHandler(tdHandler), controllerHandler(controllerHandler) {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto excludedChatsVector = (settings->GetChatsGroupType() == ChatsGroupType::include_all ? settings->GetExcludeChats() : settings->GetIncludeChats());

    for (decltype(excludedChatsVector)::reverse_iterator chatid = excludedChatsVector.rbegin(); chatid != excludedChatsVector.rend();
        ++chatid
    ) {
        auto result = chatsDb->getChatById(*chatid);
        auto chat = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>(1)->at(0);
        titles[*chatid] = chat->title;
    }
}

bool spy::service::controller::command::HelpCommand::IsDone() {
    return !this->inProcess;
}

void spy::service::controller::command::HelpCommand::Cencel() {
    this->inProcess = false;
}

void spy::service::controller::command::HelpCommand::Process(const std::string& commandLine) {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();
    auto settings = this->controllerHandler->GetController<SpySettingsController>(); 

    std::string chatGroupType = (settings->GetChatsGroupType() == ChatsGroupType::include_all ? "include all" : "exclude all");
    std::string excludeChannels = (settings->GetForceExcludeChannels() ? "exclude" : "include");
    std::string saveDeleted = (settings->GetSaveDeletedMedia() ? "save" : "ignore");
    std::string saveSecret = (settings->GetSaveSecretContent() ? "save" : "ignore");
    std::string saveSize = std::to_string(settings->GetSaveMediaSizeLimit());
    saveSize.erase(saveSize.find(".") + 2);

    // Shape excluded chats string
    std::string excludedChats;
    for (auto chat : titles) {
        excludedChats += "\n   [" + chat.second + "](tg://user?id=" + std::to_string(chat.first) + ")";
    }

    const std::string text = "" \
    "You can send case insensitive commands the same way as to bots\n\n"\
    " - `/help`\n\n"\
    "*Chats* 💬\n" \
    " - Group type: " + chatGroupType + " `/setchatgrouptype`\n" \
    " - Channels: " + excludeChannels + " `/setexcludechannels`\n" \
    " - Exclude chats: `/setexcluded`" + excludedChats + "\n\n" \

    "*Media* 📎\n" \
    " - Save deleted: " + saveDeleted + " `/savedeleted`\n" \
    " - Save secret: " + saveSecret + " `/savesecret`\n" \
    " - Size limit: " + saveSize + "mb `/savesize`";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

