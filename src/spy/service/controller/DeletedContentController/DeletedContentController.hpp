
#ifndef spy_DeletedContentController
#define spy_DeletedContentController

#include <spy/service/controller/BaseController.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

namespace spy { namespace service { namespace controller {

    class DeletedContentController : public BaseController {
    public:
        DeletedContentController() {
        }

        static const std::int32_t ID = 6151687;
        virtual std::int32_t GetControllerId() override { return DeletedContentController::ID; }

    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;

    private:
        void onUpdateDeleteMessages(td::td_api::updateDeleteMessages& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);
        void onUpdateMessageContent(td::td_api::updateMessageContent& update, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

    // private:
        // void initiateFileDonwloading(td::td_api::MessageContent& content, const std::shared_ptr<tdlpp::base::TdlppHandler>& handler);

    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_DeletedContentController
