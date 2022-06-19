
#ifndef spy_Auth
#define spy_Auth

#include <spy/auth/ConfigAuth.hpp>
#include <tdlpp/auth/DefaultAuth.hpp>

namespace spy { namespace auth {

    class SpyAuth : public tdlpp::auth::DefaultAuth {
    public:
        static std::shared_ptr<SpyAuth> create();

    public:
        SpyAuth() : tdlpp::auth::DefaultAuth() {
            configAuth = std::make_shared<ConfigAuth>();
        }

        std::shared_ptr<ConfigAuth> GetConfigAuth();

    protected:
        virtual void OnAuthStateClosed() override;
        virtual void OnAuthStateRegistration() override;
        virtual void OnAuthStateWaitCode() override;
        virtual void OnAuthStateWaitPassword() override;
        virtual void OnAuthStateWaitPhoneNumber() override;
        virtual void OnAuthStateWaitEncryptionKey() override;
        virtual void OnAuthStateWaitParametres() override;

    protected:
        std::shared_ptr<ConfigAuth> configAuth;

    };

} } // namespace spy { namespace auth


#endif // spy_Auth
