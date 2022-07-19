
#include <spy/service/controller/DeletedContentChatController/Command/SetChatGroupTypeCommand/SetChatGroupTypeCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>

bool spy::service::controller::command::SetChatGroupTypeCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SetChatGroupTypeCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);
    auto parts = StringTools::split(trimmed, ' ');


    // Default command without parammeters
    if (trimmed == "/setchatgrouptype") sendProposition();

    // Command with parammeters
    else if (parts.size() == 2 && StringTools::startsWith(trimmed, "/setchatgrouptype")) {
        if (setChatGroupType(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else if (parts.size() <= 1) {
        if (setChatGroupType(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else sendTryAgainMessage();
}

void spy::service::controller::command::SetChatGroupTypeCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled 😣, now you can use other one";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SetChatGroupTypeCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "*Parse chat group type* ✍️\n\n" \
    "Chat parsing allows you to store sensitive content (deleted or secret media) from specified group of chats. You can parse all chat or certain only.\n\n" \
    "To parse all chats send *include all*. To parse only particular ones send *exclude all* (assure to exclude chat with `/setexcluded` command). To cancel use `/cancel` command.\n\n" \
    "*Current type:*\n   ";

    std::string type = settings->GetChatsGroupType() == ChatsGroupType::include_all ? "`include all`" : "`exclude all`";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetChatGroupTypeCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" 
    "Probably there is some mistake so try again 🤔 To cancel use `/cancel` command.";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetChatGroupTypeCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved 🙂 Now you can use other commands";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SetChatGroupTypeCommand::setChatGroupType(const std::string& argument) {
    auto param = StringTools::toLower(argument);
    std::shared_ptr<ChatsGroupType> type;

    if (param.find("include") != std::string::npos) type = std::make_shared<ChatsGroupType>(ChatsGroupType::include_all);
    else if (param.find("exclude") != std::string::npos) type = std::make_shared<ChatsGroupType>(ChatsGroupType::exclude_all);

    if (type) {
        auto settings = this->controllerHandler->GetController<SpySettingsController>();
        settings->SetChatsGroupType(*type);
        return true;
    }

    return false;
}

