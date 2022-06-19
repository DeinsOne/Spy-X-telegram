
#ifndef spy_ChatsController
#define spy_ChatsController

#include <spy/service/controller/BaseController.hpp>

namespace spy { namespace service { namespace controller {

    class ChatsController : public BaseController {
    public:
        ChatsController() {
        }

        static const std::int32_t ID = 4373329;
        virtual std::int32_t GetControllerId() override { return ChatsController::ID; }

    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;

        std::vector<std::int64_t> GetAllChats();
        std::vector<std::int64_t> GetBasicGroupsChats();
        std::vector<std::int64_t> GetPrivateChats();
        std::vector<std::int64_t> GetSecretChats();
        std::vector<std::int64_t> GetSupergroupChats();
        std::vector<std::int64_t> GetChannelChats();

    protected:
        std::vector<std::int64_t> allChats;
        std::vector<std::int64_t> basicGroupsChats;
        std::vector<std::int64_t> privateChats;
        std::vector<std::int64_t> secretChats;
        std::vector<std::int64_t> supergroupChats;
        std::vector<std::int64_t> channelChats;

    private:
        void InsertChat(const std::int64_t& chat_id, td::td_api::ChatType& type);
        void InsertChat(td::td_api::chat& origin);

    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_ChatsController
