
#ifndef spy_DeletedContentChatSaveSizeCommand
#define spy_DeletedContentChatSaveSizeCommand

#include <memory>
#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace controller { namespace command {

    class SaveSizeCommand : public ICommand {
    public:
        // Function to instantiate inheritors
        static std::shared_ptr<ICommand> makeCommand(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
            const std::shared_ptr<ControllersHandler>& controllerHandler
        ) {
            auto cpmmand = std::make_shared<SaveSizeCommand>();
            cpmmand->tdHandler = tdHandler;
            cpmmand->controllerHandler = controllerHandler;
            return cpmmand;
        }

    public:
        virtual void Process(const std::string& commandLine) override;

        virtual void Cencel() override;
        virtual bool IsDone() override;

        static const std::int64_t ID = 4907250;
        virtual std::int64_t get_id() { return SaveSizeCommand::ID; };

    private:
        void sendProposition();
        void sendTryAgainMessage();
        void sendSuccessMessage();
        bool setSaveDeleted(const std::string& argument);

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> tdHandler;
        std::shared_ptr<ControllersHandler> controllerHandler;

        bool inProcess{true};
        bool skipMessage;
    };

} } } }

#endif // spy_DeletedContentChatSaveSizeCommand
