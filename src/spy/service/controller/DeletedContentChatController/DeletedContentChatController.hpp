
#ifndef spy_DeletedContentChatController
#define spy_DeletedContentChatController

#include <spy/service/controller/BaseController.hpp>

#define SPY_DELETED_CONTENT_CHAT_TITLE "Spy X telegram"
#define SPY_DELETED_CONTENT_CHAT_DESCRIPTION "Spy X telegram is an open source desktop telegram client with additional features which is focused on security and speed\nIn this chat you can manage its settings"

#include <spy/service/controller/DeletedContentChatController/Command/ICommand.hpp>

namespace spy { namespace service { namespace controller {

    class DeletedContentChatController : public BaseController {
    public:
        DeletedContentChatController() : deletedContentSupergroupChatId(0) {
        }

        static const std::int32_t ID = 6491578;
        virtual std::int32_t GetControllerId() override { return DeletedContentChatController::ID; }

    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;

        std::int64_t GetDeletedContentChatId();

    private:
        void FindDeletedContentChat(const std::vector<std::int64_t>& chat_ids, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void CreateDeletedContentChat(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void GetDeletedContentChatOwner(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void ExcludeParwingDeletedContentChat(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

        void onUpdateNewMessage(td::td_api::updateNewMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

    protected:
        std::int64_t deletedContentSupergroupId;
        std::int64_t deletedContentSupergroupChatId;

        bool canProcessCommands{false};
        std::shared_ptr<command::ICommand> command;

    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_DeletedContentChatController
