
#include <spy/service/controller/DeletedContentChatController/Command/SaveSizeCommand/SaveSizeCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>
#include <regex>

bool spy::service::controller::command::SaveSizeCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SaveSizeCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);

    // Default command without parammeters
    if (trimmed == "/savesize") sendProposition();

    else if (StringTools::startsWith(trimmed, "/savesize")) {
        if (setSaveDeleted(trimmed.substr(sizeof("/savesize ") - 1))) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }

    else {
        if (setSaveDeleted(trimmed)) {
            this->inProcess = false;
            sendSuccessMessage();
        }
        else sendTryAgainMessage();
    }
}

void spy::service::controller::command::SaveSizeCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled 🤧, now you can use other one";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SaveSizeCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Save size 💿\n\n" \
    "Setting save size you will limitate downloadable media size to reduce storage overflow.\n\n" \
    "To set downloadable media size sent message in format: *10mb* or *10 mb*. To cancel use `/cancel` command.\n\n" \
    "*Carrent limit*:\n   ";

    std::string type = "";
    if (settings->GetSaveMediaSizeLimit() < 1) {
        auto number = std::to_string(settings->GetSaveMediaSizeLimit() * 1024);
        type = "`" + number.substr(0, number.find('.') + 2) + "kb`";
    }
    else {
        auto number = std::to_string(settings->GetSaveMediaSizeLimit());
        type = "`" + number.substr(0, number.find('.') + 2) + "mb`";
    }

    functions::SendMessage sendMessage(text + type, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveSizeCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" 
    "Probably there is some mistake so try again 😔 To cancel use `/cancel` command.";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SaveSizeCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved 🙃 Now you can use other commands";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SaveSizeCommand::setSaveDeleted(const std::string& argument) {
    auto param = StringTools::toLower(argument);

    // Read megabytes
    if (std::regex_match(param, std::regex("(\\d+)(\\s*)(mb|Mb|MB)"))) {
        float number = std::stof(std::regex_replace(param, std::regex("[a-z]*"), ""));
        if (number <= 0) return false;

        this->controllerHandler->GetController<SpySettingsController>()->SetSaveMediaSizeLimit(number);
        return true;
    }

    // Read kilobytes
    else if (std::regex_match(param, std::regex("(\\d+)(\\s*)(kb|Kb|KB)"))) {
        float number = std::stof(std::regex_replace(param, std::regex("[a-z]*"), ""));
        if (number <= 0) return false;

        this->controllerHandler->GetController<SpySettingsController>()->SetSaveMediaSizeLimit(number / 1024);
        return true;
    }

    return false;
}

