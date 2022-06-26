
#ifndef spy_DeletedContentChatHelpCommand
#define spy_DeletedContentChatHelpCommand

#include <memory>
#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace controller { namespace command {

    class HelpCommand : public ICommand {
    public:
        // Function to instantiate inheritors
        static std::shared_ptr<ICommand> makeCommand(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
            const std::shared_ptr<ControllersHandler>& controllerHandler
        ) {
            auto cpmmand = std::make_shared<HelpCommand>();
            cpmmand->tdHandler = tdHandler;
            cpmmand->controllerHandler = controllerHandler;
            return cpmmand;
        }

    public:
        virtual bool IsDone() override;

        virtual void Cencel() override;

        virtual void Process(const std::string& commandLine) override;

        static const std::int64_t ID = 64324454;
        virtual std::int64_t get_id() { return HelpCommand::ID; };

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> tdHandler;
        std::shared_ptr<ControllersHandler> controllerHandler;

        bool inProcess{true};
    };

} } } }

#endif // spy_DeletedContentChatHelpCommand
