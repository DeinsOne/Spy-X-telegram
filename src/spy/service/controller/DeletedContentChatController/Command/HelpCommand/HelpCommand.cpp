
#include <spy/service/controller/DeletedContentChatController/Command/HelpCommand/HelpCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>


bool spy::service::controller::command::HelpCommand::IsDone() {
    return !this->inProcess;
}

void spy::service::controller::command::HelpCommand::Cencel() {
    this->inProcess = false;
}

void spy::service::controller::command::HelpCommand::Process(const std::string& commandLine) {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    const std::string text = "" \
    "*'Deleted Content #spy'* is a supergroup " \
    "created by *TelegramSpy* application, for " \
    "notifying user about deleted content such as " \
    "photos, videos, audio and video notes.\n\n" \
    "You can send case insensitive commands the " \
    "same way as to bots.\n\n" \
    "*Commands list*:\n" \
    " - `/setchatgrouptype`\n" \
    " - `/setexcludechannels`\n" \
    " - `/savedeleted`\n" \
    " - `/cancel`\n" \
    " - `/help`";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

