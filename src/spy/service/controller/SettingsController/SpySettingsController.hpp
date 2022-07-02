
#ifndef spy_SpySettingsController
#define spy_SpySettingsController

#include <spy/service/controller/BaseController.hpp>

namespace spy { namespace service { namespace controller {

    enum class ChatsGroupType {
        include_all = 0,
        exclude_all
    };

    class SpySettingsController : public BaseController {
    public:
        SpySettingsController() : chatsGroupType(ChatsGroupType::include_all), saveMediaSizeLimit(50) {
        }

        static const std::int32_t ID = 6964377;
        virtual std::int32_t GetControllerId() override { return SpySettingsController::ID; }

    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) override;

        // Getters
        ChatsGroupType GetChatsGroupType();
        std::vector<std::int64_t> GetIncludeChats();
        std::vector<std::int64_t> GetExcludeChats();
        bool GetForceExcludeChannels();

        bool GetSaveDeletedMedia();
        bool GetSaveSecretContent();
        float GetSaveMediaSizeLimit();

        // Setters
        void SetChatsGroupType(const ChatsGroupType& type);
        void SetIncludeChats(const std::vector<std::int64_t>& chats);
        void SetExcludeChats(const std::vector<std::int64_t>& chats);
        void SetForceExcludeChannels(const bool& excludeChanels);

        void SetSaveDeletedMedia(const bool& save);
        void SetSaveSecretContent(const bool& save);
        void SetSaveMediaSizeLimit(const float& size);

    private:
        void ReadSettings(const std::string& path);
        void WriteSettings(const std::string& path);

        void onUpdateNewMessage(td::td_api::updateNewMessage& update);

    protected:
        ChatsGroupType chatsGroupType;
        std::vector<std::int64_t> includeChats;     // Works only if is exclude_all chats group type
        std::vector<std::int64_t> excludeChats;     // Works only if is include_all chats group type.
        bool forceExcludeChannels{true};

        bool saveDeletedMedia{true};
        bool saveSecretContent{true};
        float saveMediaSizeLimit;
    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_SpySettingsController
