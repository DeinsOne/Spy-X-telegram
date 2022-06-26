
#ifndef spy_DeletedContentChatCommandHandler
#define spy_DeletedContentChatCommandHandler

#include <memory>
#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace controller { namespace command {

    /**
     * @brief Get the Command name from command line
     * 
     * @param commandLine String of '/command arg1 arg2..' form
     * @return std::string String of '/command' form
     */
    std::string getCommandName(const std::string& commandLine);

    /**
     * @brief Creates command according to its name. Retrurn nullptr if command
     * was not added to comand-id bindings list
     * 
     * @param commandLine String of '/command arg1 arg2..' form
     * @param tdHandler TdlppHandler
     * @param controllerHandler ControllersHandler
     * @return std::shared_ptr<ICommand> Command object
     */
    std::shared_ptr<command::ICommand> matchCreateCommand(
        const std::string& commandLine,
        const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
        const std::shared_ptr<ControllersHandler>& controllerHandler
    );

} } } }

#endif // spy_DeletedContentChatCommandHandler
