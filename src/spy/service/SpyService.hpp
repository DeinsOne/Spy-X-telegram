
#ifndef spy_Service
#define spy_Service

#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/service/controller/ControllersHandler.hpp>

namespace spy { namespace service {

    class SpyService {
    public:
        SpyService(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            const std::shared_ptr<controller::ControllersHandler>& controllersHandler
        );

        static std::shared_ptr<SpyService> CreateShared(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            const std::shared_ptr<controller::ControllersHandler>& controllersHandler
        );

    public:

        /**
         * @brief Registers telegram updates handler for current TdlppHandler
         */
        // void addUpdates();

    private:
        //========== Message updates
        // void onUpdateNewMessage(td::td_api::updateNewMessage update);
        // void onUpdateDeleteMessages(td::td_api::updateDeleteMessages update);
        // void onUpdateMessageContent(td::td_api::updateMessageContent update);
        // void onUpdateMessageEdited(td::td_api::updateMessageEdited update);
        // void onUpdateChatDraftMessage(td::td_api::updateChatDraftMessage update);

        //========== Users updates
        // void onUpdateUser(td::td_api::updateUser update);
        // void onUpdateUserFullInfo(td::td_api::updateUserFullInfo update);
        // void onUpdateUserStatus(td::td_api::updateUserStatus update);

        //========== Chat updates
        // void onUpdateNewChat(td::td_api::updateNewChat update);
        // void onUpdateChatLastMessage(td::td_api::updateChatLastMessage update);
        // void onUpdateChatMember(td::td_api::updateChatMember update);
        // void onUpdateChatPermissions(td::td_api::updateChatPermissions update);
        // void onUpdateChatPhoto(td::td_api::updateChatPhoto update);
        // void onUpdateChatTitle(td::td_api::updateChatTitle update);
        // void onUpdateSecretChat(td::td_api::updateSecretChat update);

        //========== Groups updates
        // void onUpdateBasicGroup(td::td_api::updateBasicGroup update);
        // void onUpdateBasicGroupFullInfo(td::td_api::updateBasicGroupFullInfo update);
        // void onUpdateSupergroup(td::td_api::updateSupergroup update);
        // void onUpdateSupergroupFullInfo(td::td_api::updateSupergroupFullInfo update);

        //========== Ungrouped updates
        // void onUpdateOption(td::td_api::updateOption update);
        // void onUpdateFile(td::td_api::updateFile update);

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::shared_ptr<tdlpp::base::TdlppHandler> tdlppHandler;
        std::shared_ptr<controller::ControllersHandler> controllersHandler;

    };

} } // namespace spy { namespace { service


#endif // spy_Service
