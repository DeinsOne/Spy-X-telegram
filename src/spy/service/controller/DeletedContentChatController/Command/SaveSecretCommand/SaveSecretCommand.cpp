
#include <spy/service/controller/DeletedContentChatController/Command/SaveSecretCommand/SaveSecretCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>

bool spy::service::controller::command::SaveSecretCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SaveSecretCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);
    auto parts = StringTools::split(trimmed, ' ');


    // Default command without parammeters
    if (trimmed == "/savesecret") sendProposition();

    // Command with parammeters
    else if (parts.size() == 2 && StringTools::startsWith(trimmed, "/savesecret")) {
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

void spy::service::controller::command::SaveSecretCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled, now you can use other one";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SaveSecretCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "To enable saving of secret content send `save`/`enable`. To disable send `ignore`/`disable`\n\n" \
    "*Save secret*: ";

    std::string type = settings->GetSaveSecretContent() ? "enable" : "disable";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveSecretCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Bad input for /savesecret. To cancel this command send `/cancel`, or try again";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveSecretCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved. Now you can use other commands\n\n" \
    "*Save secret*: ";

    std::string type = settings->GetSaveSecretContent() ? "enable" : "disable";

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SaveSecretCommand::setSaveDeleted(const std::string& argument) {
    auto param = StringTools::toLower(argument);

    if (param == "on" || param == "true" || param == "enable" || param == "save" || param == "1") {
        this->controllerHandler->GetController<SpySettingsController>()->SetSaveSecretContent(true);
        return true;
    }
    else if (param == "off" || param == "false" || param == "disable" || param == "ignore" || param == "0") {
        this->controllerHandler->GetController<SpySettingsController>()->SetSaveSecretContent(false);
        return true;
    }

    return false;
}

