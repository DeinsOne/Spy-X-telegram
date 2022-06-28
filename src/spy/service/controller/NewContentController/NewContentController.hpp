
#ifndef spy_NewContentController
#define spy_NewContentController

#include <spy/service/controller/BaseController.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <spy/db/ChatsDatabase/ChatsDatabase.hpp>

namespace spy { namespace service { namespace controller {

    class NewContentController : public BaseController {
    public:
        NewContentController() {
        }

        static const std::int32_t ID = 6802023;
        virtual std::int32_t GetControllerId() override { return NewContentController::ID; }

    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;

    private:
        void onUpdateNewMessage(td::td_api::updateNewMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void onUpdateFile(td::td_api::updateFile& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void onUpdateChatLastMessage(td::td_api::updateChatLastMessage& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);
        OATPP_COMPONENT(std::shared_ptr<db::ChatsDatabase>, chatsDb);

    private:
        void initiateFileDonwloading(td::td_api::MessageContent& content, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_SpySettingsController
