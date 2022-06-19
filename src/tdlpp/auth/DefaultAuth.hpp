
#ifndef tdlpp_DefaultAuth
#define tdlpp_DefaultAuth

#include <tdlpp/auth/IAuth.hpp>
#include <condition_variable>
#include <mutex>

namespace tdlpp { namespace auth {

    class DefaultAuth : public IAuth, public TdlppBase {
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<DefaultAuth> New instance of 'DefaultAuth'
         */
        static std::shared_ptr<DefaultAuth> create();
        /**
         * @brief A factory method
         * @details You can generate api on https://my.telegram.org
         * 
         * @param tdApiId_ Your Telegram API ID.
         * @param tdApiHash_ This is the API hash that you get from the Telegram API website.
         * @param tdDatabaseDir_ The directory where the TDLib database will be stored.
         * @param deviceModel_ The name of the device model.
         * @param applicationVersion_ The version of your application.
         * @param applicationLanguageCode_ The language code of the application.
         * 
         * @return A shared pointer to a DefaultAuth object.
         */
        static std::shared_ptr<DefaultAuth> create(
            const std::int32_t& tdApiId_,
            const std::string& tdApiHash_,
            const std::string& tdDatabaseDir_ = "tdlib",
            const std::string& deviceModel_ = "desktop",
            const std::string& applicationVersion_ = "1.0",
            const std::string& applicationLanguageCode_ = "en"
        );

    public:
        DefaultAuth();
        DefaultAuth(
            const std::int32_t& tdApiId_,
            const std::string& tdApiHash_,
            const std::string& tdDatabaseDir_,
            const std::string& deviceModel_,
            const std::string& applicationVersion_,
            const std::string& applicationLanguageCode_
        );

        // Check if the instance is authorized or not
        virtual bool IsAuthorized() override { return authorized; }
        // Wait for the authorization to complete, or for the maximum number of retries to be exceeded
        virtual void WaitAuthorized();

        // Returns count of consecutive retries
        virtual std::uint32_t GetRetriesCount() override { return retries; }

    protected:
        virtual void OnAuthorized() override;
        virtual void OnLogOut() override;

        virtual void OnAuthStateClosed() override;
        virtual void OnAuthStateRegistration() override;
        virtual void OnAuthStateWaitCode() override;
        virtual void OnAuthStateWaitPassword() override;
        virtual void OnAuthStateWaitPhoneNumber() override;
        virtual void OnAuthStateWaitEncryptionKey() override;
        virtual void OnAuthStateWaitParametres() override;

        virtual void OnAuthStateUpdate(td::td_api::updateAuthorizationState&&) override;

    protected:
        virtual void SetHandler(const std::shared_ptr<base::TdlppHandler>& handler);

        virtual bool HandleRetry() override;
        void handleUpdate();

    protected:
        std::uint64_t authQueryId{0};
        bool authorized{false};

        std::int32_t tdApiId{-1};
        std::string tdApiHash;
        std::string tdDatabaseDir;
        std::string deviceModel;
        std::string applicationVersion;
        std::string applicationLanguageCode;

        bool retry{false};
        std::uint32_t retries{0};

        SharedObjectPtr<td::td_api::AuthorizationState> authState;
        std::condition_variable authLock;

        std::shared_ptr<base::TdlppHandler> handler_;
    };

} // namespace auth
} // namespace tdlpp

#endif // tdlpp_DefaultAuth
