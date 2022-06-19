
#include <tdlpp/auth/DefaultAuth.hpp>
#include <tdlpp/base/Handler.hpp>
#include <td/tl/TlObject.h>

#include <tdlpp/auth/utils.hpp>

std::shared_ptr<tdlpp::auth::DefaultAuth> tdlpp::auth::DefaultAuth::create() {
    return std::make_shared<tdlpp::auth::DefaultAuth>();
}

std::shared_ptr<tdlpp::auth::DefaultAuth> tdlpp::auth::DefaultAuth::create(
    const std::int32_t& tdApiId_,
    const std::string& tdApiHash_,
    const std::string& tdDatabaseDir_,
    const std::string& deviceModel_,
    const std::string& applicationVersion_,
    const std::string& applicationLanguageCode_
) {
    return std::make_shared<tdlpp::auth::DefaultAuth>(tdApiId_, tdApiHash_, tdDatabaseDir_, deviceModel_, applicationVersion_, applicationLanguageCode_);
}

tdlpp::auth::DefaultAuth::DefaultAuth()
 : tdApiId(-1), tdApiHash(""), tdDatabaseDir("tdlib"), deviceModel("desktop"), applicationVersion("1.0"), applicationLanguageCode("en")
{
    TDLPP_LOG_VERBOSE("constructor default");
}

tdlpp::auth::DefaultAuth::DefaultAuth(
    const std::int32_t& tdApiId_,
    const std::string& tdApiHash_,
    const std::string& tdDatabaseDir_,
    const std::string& deviceModel_,
    const std::string& applicationVersion_,
    const std::string& applicationLanguageCode_
) : tdApiId(tdApiId_), tdApiHash(tdApiHash_), tdDatabaseDir(tdDatabaseDir_), deviceModel(deviceModel_), applicationVersion(applicationVersion_),
    applicationLanguageCode(applicationLanguageCode_)
{
    TDLPP_LOG_VERBOSE("constructor parametrized custom");
}

void tdlpp::auth::DefaultAuth::WaitAuthorized() {
    TDLPP_LOG_DEBUG("lock");
    std::mutex _mtx;
    std::unique_lock<std::mutex> lock(_mtx);
    authLock.wait(lock, [&] { return authorized || retries >= TDLPP_MAX_AUTH_RETRIES; });
    TDLPP_LOG_DEBUG("unlock");
}

void tdlpp::auth::DefaultAuth::handleUpdate() {
    authQueryId++;
    TDLPP_LOG_DEBUG("%s", TDLPP_TD_ID_NAME(authState->get_id()));

    td::td_api::downcast_call(*authState, overloaded(
        [this](td::td_api::authorizationStateWaitTdlibParameters& object) {
            OnAuthStateWaitParametres();
        },
        [this](td::td_api::authorizationStateWaitEncryptionKey& object) {
            OnAuthStateWaitEncryptionKey();
        },
        [this](td::td_api::authorizationStateWaitPhoneNumber& object) {
            OnAuthStateWaitPhoneNumber();
        },
        [this](td::td_api::authorizationStateWaitCode& object) {
            OnAuthStateWaitCode();
        },
        [this](td::td_api::authorizationStateWaitRegistration& object) {
            OnAuthStateRegistration();
        },
        [this](td::td_api::authorizationStateWaitPassword& object) {
            OnAuthStateWaitPassword();
        },
        [this](td::td_api::authorizationStateReady& object) {
            OnAuthorized();
        },
        [this](td::td_api::authorizationStateLoggingOut& object) {
            OnLogOut();
        },
        [this](td::td_api::authorizationStateClosed& object) {
            OnAuthStateClosed();
        },
        [this](auto&) {}
    ));
}

void tdlpp::auth::DefaultAuth::OnAuthStateUpdate(td::td_api::updateAuthorizationState&& update) {
    authState = SharedObjectPtr<td::td_api::AuthorizationState>(std::move(update.authorization_state_).release());

    handleUpdate();
}

void tdlpp::auth::DefaultAuth::OnAuthorized() {
    TDLPP_LOG_DEBUG(" ");
    authorized = true;
    authLock.notify_all();
}

void tdlpp::auth::DefaultAuth::OnLogOut() {
    TDLPP_LOG_ERROR(" ");
    authorized = false;
    authLock.notify_all();
}

void tdlpp::auth::DefaultAuth::OnAuthStateClosed() {
    TDLPP_LOG_ERROR(" ");
    authorized = false;
    authLock.notify_all();
}

void tdlpp::auth::DefaultAuth::OnAuthStateWaitCode() {
    TDLPP_LOG_DEBUG(" ");

    std::cout << "  Enter authentication code: " << std::flush;
    std::string code;
    std::cin >> code;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkAuthenticationCode>(code),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [this](auto&) {}
            ));
        }
    );
}

void tdlpp::auth::DefaultAuth::OnAuthStateRegistration() {
    TDLPP_LOG_DEBUG(" ");

    std::string first_name;
    std::string last_name;
    std::cout << "  Enter your first name: " << std::flush;
    std::cin >> first_name;
    std::cout << "  Enter your last name: " << std::flush;
    std::cin >> last_name;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::registerUser>(first_name, last_name),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [](auto&) {}
            ));
        }
    );
}

void tdlpp::auth::DefaultAuth::OnAuthStateWaitPassword() {
    TDLPP_LOG_DEBUG(" ");

    std::string password = utils::getpass("  Enter authentication password: ");

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkAuthenticationPassword>(password),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [](auto&) {}
            ));
        }
    );
}

void tdlpp::auth::DefaultAuth::OnAuthStateWaitPhoneNumber() {
    TDLPP_LOG_DEBUG(" ");

    std::cout << "  Enter phone number: ";
    std::string phone_number;
    std::getline(std::cin, phone_number);


    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [](auto&) {}
            ));
        }
    );
}

void tdlpp::auth::DefaultAuth::OnAuthStateWaitEncryptionKey() {
    TDLPP_LOG_DEBUG(" ");

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkDatabaseEncryptionKey>(""),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [](auto&) {}
            ));
        }
    );
}

void tdlpp::auth::DefaultAuth::OnAuthStateWaitParametres() {
    TDLPP_LOG_DEBUG(" ");

    if (tdApiId <= 0) {
        printf("You can generate api id and hash on 'https://my.telegram.org'\n");
        tdApiId = std::stoi(utils::getpass("  Enter api id: "));
        tdApiHash = utils::getpass("  Enter api hash: ");
    }

    auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
    parameters->database_directory_ = tdDatabaseDir;
    parameters->api_id_ = tdApiId;
    parameters->api_hash_ = tdApiHash;
    parameters->system_language_code_ = applicationLanguageCode;
    parameters->device_model_ = deviceModel;
    parameters->application_version_ = applicationVersion;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::setTdlibParameters>(std::move(parameters)),
        [this](SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, overloaded(
                [this](td::td_api::error& err) {
                    TDLPP_LOG_ERROR("%s", err.message_.c_str());
                    retry = true;
                    ++retries;
                },
                [this](td::td_api::ok&) {
                    retries = 0;
                },
                [](auto&) {}
            ));
        }
    );
}


void tdlpp::auth::DefaultAuth::SetHandler(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
    TDLPP_LOG_VERBOSE(" ");
    handler_ = handler;
}

bool tdlpp::auth::DefaultAuth::HandleRetry() {
    if (retry) {
        handleUpdate();
        retry = false;
        authLock.notify_all();
        return true;
    }

    return false;
}
