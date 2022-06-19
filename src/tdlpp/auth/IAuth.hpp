
#ifndef tdlpp_IAuth
#define tdlpp_IAuth

#include <tdlpp/core.hpp>

namespace tdlpp {
    namespace base {
        class TdlppHandler;
    } // namespace base
    namespace poll {
        class PollManager;
    } // namespace poll
} // namespace tdlpp

namespace tdlpp { namespace auth {

    // An interface for the authorization process
    class IAuth {
    public:
        // Check if the instance is authorized or not
        virtual bool IsAuthorized() = 0;
        // Block thread until authorized
        virtual void WaitAuthorized() = 0;

        // Returns count of consecutive retries
        virtual std::uint32_t GetRetriesCount() = 0;

    protected:
        // Callback on authorizationStateReady
        virtual void OnAuthorized() = 0;
        // Callback on authorizationStateLoggingOut
        virtual void OnLogOut() = 0;

        // Callback on authorizationStateLoggingOut
        virtual void OnAuthStateClosed() = 0;
        // Callback on authorizationStateWaitCode
        virtual void OnAuthStateWaitCode() = 0;
        // Callback on authorizationStateWaitRegistration
        virtual void OnAuthStateRegistration() = 0;
        // Callback on authorizationStateWaitPassword
        virtual void OnAuthStateWaitPassword() = 0;
        // Callback on authorizationStateWaitPhoneNumber
        virtual void OnAuthStateWaitPhoneNumber() = 0;
        // Callback on authorizationStateWaitEncryptionKey
        virtual void OnAuthStateWaitEncryptionKey() = 0;
        // Callback on authorizationStateWaitTdlibParameters
        virtual void OnAuthStateWaitParametres() = 0;

        // Callback on updateAuthorizationState
        virtual void OnAuthStateUpdate(td::td_api::updateAuthorizationState&&) = 0;

    protected:
        friend base::TdlppHandler;
        /**
         * @brief Set the Handler object
         * 
         * @param handler Actual handler
         */
        virtual void SetHandler(const std::shared_ptr<base::TdlppHandler>& handler) = 0;

        friend poll::PollManager;
        /**
         * @brief Callback retry
         * 
         * @return True or false whether retry was handled
         */
        virtual bool HandleRetry() = 0;
    };

} // namespace auth
} // namespace tdlpp

#endif // tdlpp_IAuth
