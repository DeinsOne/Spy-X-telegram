
#include <unordered_map>
#include <spy/service/controller/DeletedContentChatController/Command/CommandHandler.hpp>
#include <spy/utils/StringTools.h>

#include <spy/service/controller/DeletedContentChatController/Command/HelpCommand/HelpCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/SetChatGroupTypeCommand/SetChatGroupTypeCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/SetExcludeChannelsCommand/SetExcludeChannelsCommand.hpp>
#include <spy/service/controller/DeletedContentChatController/Command/SaveDeletedCommand/SaveDeletedCommand.hpp>


std::string spy::service::controller::command::getCommandName(const std::string& commandLine) {
    if (commandLine.find(' ') == std::string::npos) return commandLine;

    auto parts = StringTools::split(commandLine, ' ');
    return parts.front();
}

std::shared_ptr<spy::service::controller::command::ICommand> spy::service::controller::command::matchCreateCommand(
    const std::string& commandLine,
    const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
    const std::shared_ptr<ControllersHandler>& controllerHandler
) {
    // Static id bindings
    static const std::unordered_map<std::string, std::int64_t> nameToIdBindings = {
        { "/help", spy::service::controller::command::HelpCommand::ID },
        { "/setchatgrouptype", spy::service::controller::command::SetChatGroupTypeCommand::ID },
        { "/setexcludechannels", spy::service::controller::command::SetExcludeChannelsCommand::ID },
        { "/savedeleted", spy::service::controller::command::SaveDeletedCommand::ID }
        // TODO: add new name-class binding here
    };

    auto idIter = nameToIdBindings.find(getCommandName(commandLine));

    // Return no command if binding is not found
    if (idIter == nameToIdBindings.end()) return nullptr;

    // Process binding
    switch (idIter->second) {
        case (spy::service::controller::command::HelpCommand::ID): {
            return spy::service::controller::command::HelpCommand::makeCommand(tdHandler, controllerHandler);
        }
        case (spy::service::controller::command::SetChatGroupTypeCommand::ID): {
            return spy::service::controller::command::SetChatGroupTypeCommand::makeCommand(tdHandler, controllerHandler);
        }
        case (spy::service::controller::command::SetExcludeChannelsCommand::ID): {
            return spy::service::controller::command::SetExcludeChannelsCommand::makeCommand(tdHandler, controllerHandler);
        }
        case (spy::service::controller::command::SaveDeletedCommand::ID): {
            return spy::service::controller::command::SaveDeletedCommand::makeCommand(tdHandler, controllerHandler);
        }
        // TODO: add new name-class binding here
    }

    // Rreturn empty command if is not found
    return nullptr;
}

