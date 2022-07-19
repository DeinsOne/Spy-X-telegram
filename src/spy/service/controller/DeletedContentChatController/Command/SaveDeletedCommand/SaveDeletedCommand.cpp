
#include <spy/service/controller/DeletedContentChatController/Command/SaveDeletedCommand/SaveDeletedCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>

bool spy::service::controller::command::SaveDeletedCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SaveDeletedCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);
    auto parts = StringTools::split(trimmed, ' ');


    // Default command without parammeters
    if (trimmed == "/savedeleted") sendProposition();

    // Command with parammeters
    else if (parts.size() == 2 && StringTools::startsWith(trimmed, "/savedeleted")) {
        if (setSaveDeleted(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else if (parts.size() <= 1) {
        if (setSaveDeleted(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else sendTryAgainMessage();
}

void spy::service::controller::command::SaveDeletedCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled 🤨, now you can use other one";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SaveDeletedCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "*Save deleted* 🗑\n\n" \
    "You can control what to do with deleted media. If you choose save, all the media will be saved to have locally copy once deleted.\n\n" \
    "To save deleted send `save` or `ignore`. To cancel use `/cancel` command.\n\n" \
    "*Carrent behaviour:*\n   ";

    std::string type = settings->GetSaveDeletedMedia() ? "`save`" : "`ignore`";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveDeletedCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" 
    "Probably there is some mistake so try again 😦 To cancel use `/cancel` command.";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveDeletedCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved 🤤 Now you can use other commands";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SaveDeletedCommand::setSaveDeleted(const std::string& argument) {
    auto param = StringTools::toLower(argument);

    if (param == "on" || param == "true" || param == "enable" || param == "save" || param == "1") {
        this->controllerHandler->GetController<SpySettingsController>()->SetSaveDeletedMedia(true);
        return true;
    }
    else if (param == "off" || param == "false" || param == "disable" || param == "ignore" || param == "0") {
        this->controllerHandler->GetController<SpySettingsController>()->SetSaveDeletedMedia(false);
        return true;
    }

    return false;
}

