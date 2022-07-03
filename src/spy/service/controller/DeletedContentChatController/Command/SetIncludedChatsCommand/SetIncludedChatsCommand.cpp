
#include <spy/service/controller/DeletedContentChatController/Command/SetIncludedChatsCommand/SetIncludedChatsCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>

#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/ChatsController/ChatsController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

#include <spy/service/functions/SendMessage/SendMessage.hpp>


spy::service::controller::command::SetIncludedChatsCommand::SetIncludedChatsCommand(
    const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
    const std::shared_ptr<ControllersHandler>& controllerHandler
) : tdHandler(tdHandler), controllerHandler(controllerHandler) {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto includedChats = settings->GetIncludeChats();

    for (auto chatid : includedChats) {
        auto result = chatsDb->getChatById(chatid);
        auto chat = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>(1)->at(0);
        titles[chatid] = chat->title;
    }
}



bool spy::service::controller::command::SetIncludedChatsCommand::IsDone() {
    return !this->inProcess;
}


void spy::service::controller::command::SetIncludedChatsCommand::Process(const std::string& commandLine) {
    if (skipMessage) {
        skipMessage = false;
        return;
    }

    auto trimmed = StringTools::trim(commandLine);

    // Default command without parammeters
    if (trimmed == "/setincluded") sendProposition();

    else if (StringTools::startsWith(trimmed, "/setincluded") && trimmed.find(' ') != std::string::npos) {
        // find command's argument
        auto argument = trimmed.substr(trimmed.find(' ') + 1);

        if (setIncludedChats(argument)) {
            this->inProcess = false;
            sendSuccessMessage();
        }
    }

    else if (StringTools::startsWith(trimmed, "/setincluded") && trimmed.find(' ') == std::string::npos) {
        sendTryAgainMessage();
    }

    else {
        if (setIncludedChats(trimmed)) {
            this->inProcess = false;
            sendSuccessMessage();
        }   
    }
}

void spy::service::controller::command::SetIncludedChatsCommand::Cencel() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "The command was cancelled, now you can use other one";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    this->inProcess = false;
}

void spy::service::controller::command::SetIncludedChatsCommand::sendProposition() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "To include chat to parsing send it's title or username. Relax, system tryes to identify best match. If a chat is already included, send its title to exclude\n\n" \
    "*Current include*:\n";

    auto includedChats = settings->GetIncludeChats();
    for (auto chatid : includedChats) {
        auto result = chatsDb->getChatById(chatid);
        auto chat = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>(1)->at(0);
        text += "`" + *chat->title + "`\n";
    }

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetIncludedChatsCommand::sendTryAgainMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" 
    "Bad input for /setincluded. To cancel this command send `/cancel`, or try again";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetIncludedChatsCommand::sendNotFoundMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Sorry, caht not found. To cancel this command send `/cancel`, or try again";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetIncludedChatsCommand::sendSuggestionsMessage() {
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Sorry, there is too much suggestions. It is probably internal error. To cancel this command send `/cancel`, or try again";

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

void spy::service::controller::command::SetIncludedChatsCommand::sendSuccessMessage() {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto deletedChatController = this->controllerHandler->GetController<DeletedContentChatController>();

    std::string text = "" \
    "Edit successfully saved. Now you can use other commands\n\n" \
    "*Current include*:\n";

    auto includedChats = settings->GetIncludeChats();
    for (auto chatid : includedChats) {
        auto result = chatsDb->getChatById(chatid);
        auto chat = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>(1)->at(0);
        text += "`" + *chat->title + "`\n";
    }

    functions::SendMessage sendMessage(text, deletedChatController->GetDeletedContentChatId(), this->tdHandler);
    sendMessage.Execute();

    skipMessage = true;
}

bool spy::service::controller::command::SetIncludedChatsCommand::setIncludedChats(const std::string& argument) {
    auto settings = this->controllerHandler->GetController<SpySettingsController>();
    auto includedChats = settings->GetIncludeChats();

    // Remove excluded chat    
    for (auto i : titles) {
        if (i.second == argument) {
            auto toRemove = std::find(includedChats.begin(), includedChats.end(), i.first);
            if (toRemove != includedChats.end()) {
                includedChats.erase(toRemove);
                settings->SetIncludeChats(includedChats);
            }

            return true;
        }
    }


    // Exclude chat
    auto result = chatsDb->executeQuery(
        "SELECT * FROM chats WHERE title=:title;",
        {
            { "title", oatpp::String(argument) }
        }
    );

    auto chats = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>();

    if (chats->size() > 1) {
        sendSuggestionsMessage();
        return false;
    }

    else if (chats->size() == 0) {
        sendNotFoundMessage();
        return false;
    }

    includedChats.emplace_back((int)chats->at(0)->id);
    settings->SetIncludeChats(includedChats);

    return true;
}
