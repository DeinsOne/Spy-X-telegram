
#ifndef spy_DeletedContentChatSetExcludedChatsCommand
#define spy_DeletedContentChatSetExcludedChatsCommand

#include <memory>
#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

#include <spy/service/controller/ControllersHandler.hpp>
#include <tdlpp/base/Handler.hpp>

#include <spy/db/ChatsDatabase/ChatsDatabase.hpp>

namespace spy { namespace service { namespace controller { namespace command {

    class SetExcludedChatsCommand : public ICommand {
    public:
        // Function to instantiate inheritors
        static std::shared_ptr<ICommand> makeCommand(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
            const std::shared_ptr<ControllersHandler>& controllerHandler
        ) {
            return std::make_shared<SetExcludedChatsCommand>(tdHandler, controllerHandler);
        }

        SetExcludedChatsCommand(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& tdHandler,
            const std::shared_ptr<ControllersHandler>& controllerHandler
        );

    public:
        virtual void Process(const std::string& commandLine) override;

        virtual void Cencel() override;
        virtual bool IsDone() override;

        static const std::int64_t ID = 4816150;
        virtual std::int64_t get_id() { return SetExcludedChatsCommand::ID; };

    private:
        void sendProposition();
        void sendSuccessMessage();
        bool setExcludedChats(const std::string& argument);

        void sendNotFoundMessage();
        void sendSuggestionsMessage();
        void sendTryAgainMessage();

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> tdHandler;
        std::shared_ptr<ControllersHandler> controllerHandler;

        ChatsGroupType chatsGroup;
        std::unordered_map<std::int64_t, std::string> titles;

        bool inProcess{true};
        bool skipMessage{false};

    private:
        OATPP_COMPONENT(std::shared_ptr<db::ChatsDatabase>, chatsDb);

    };

} } } }

#endif // spy_DeletedContentChatSetExcludedChatsCommand
