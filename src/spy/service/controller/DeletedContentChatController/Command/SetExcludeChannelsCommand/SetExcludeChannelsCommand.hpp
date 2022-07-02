
#ifndef spy_DeletedContentChatSetExcludeChannelsCommand
#define spy_DeletedContentChatSetExcludeChannelsCommand

#include <memory>
#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace controller { namespace command {

    class SetExcludeChannelsCommand : public ICommand {
    public:
        // Function to instantiate inheritors
        static std::shared_ptr<ICommand> makeCommand(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
            const std::shared_ptr<ControllersHandler>& controllerHandler
        ) {
            auto cpmmand = std::make_shared<SetExcludeChannelsCommand>();
            cpmmand->tdHandler = tdHandler;
            cpmmand->controllerHandler = controllerHandler;
            return cpmmand;
        }

    public:
        virtual bool IsDone() override;

        virtual void Cencel() override;

        virtual void Process(const std::string& commandLine) override;

        static const std::int64_t ID = 18406676;
        virtual std::int64_t get_id() { return SetExcludeChannelsCommand::ID; };

    private:
        void sendProposition();
        void sendTryAgainMessage();
        void sendSuccessMessage();
        bool setExcludeChannels(const std::string& argument);

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> tdHandler;
        std::shared_ptr<ControllersHandler> controllerHandler;

        bool inProcess{true};
        bool skipMessage;
    };

} } } }

#endif // spy_DeletedContentChatSetExcludeChannelsCommand
