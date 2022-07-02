
#include <spy/service/controller/DeletedContentChatController/Command/SetExcludeChannelsCommand/SetExcludeChannelsCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>

bool spy::service::controller::command::SetExcludeChannelsCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SetExcludeChannelsCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);
    auto parts = StringTools::split(trimmed, ' ');


    // Default command without parammeters
    if (trimmed == "/setexcludechannels") sendProposition();

    // Command with parammeters
    else if (parts.size() == 2 && StringTools::startsWith(trimmed, "/setexcludechannels")) {
        if (setExcludeChannels(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else if (parts.size() <= 1) {
        if (setExcludeChannels(parts.back())) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else sendTryAgainMessage();
}

void spy::service::controller::command::SetExcludeChannelsCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled, now you can use other commands";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SetExcludeChannelsCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "To enable channels excludeing send `on`/`true`/`exclude`/`enable`. To disable send `off`/`false`/`include`/`disable`\n\n" \
    "*Exclude channels*: ";

    std::string type = settings->GetForceExcludeChannels() ? "enable" : "disable";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetExcludeChannelsCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Bad input for `/setexcludechannels`. To cancel this command send `/cancel`, or try again";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetExcludeChannelsCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved. Now you can use other commands\n\n" \
    "*Exclude channels*: ";

    std::string type = settings->GetForceExcludeChannels() ? "enable" : "disable";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SetExcludeChannelsCommand::setExcludeChannels(const std::string& argument) {
    auto param = StringTools::toLower(argument);

    if (param == "on" || param == "true" || param == "enable" || param == "exclude" || param == "1") {
        this->controllerHandler->GetController<SpySettingsController>()->SetForceExcludeChannels(true);
        return true;
    }
    else if (param == "off" || param == "false" || param == "disable" || param == "include" || param == "0") {
        this->controllerHandler->GetController<SpySettingsController>()->SetForceExcludeChannels(false);
        return true;
    }

    return false;
}

