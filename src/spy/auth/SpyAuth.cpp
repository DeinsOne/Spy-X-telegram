
#include <spy/auth/SpyAuth.hpp>

#include <tdlpp/auth/utils.hpp>
#include <tdlpp/base/Handler.hpp>


std::shared_ptr<spy::auth::SpyAuth> spy::auth::SpyAuth::create() {
    return std::make_shared<spy::auth::SpyAuth>();
}

std::shared_ptr<spy::auth::ConfigAuth> spy::auth::SpyAuth::GetConfigAuth() {
    return configAuth;
}


void spy::auth::SpyAuth::OnAuthStateWaitParametres() {
    TDLPP_LOG_DEBUG(" ");

    if (configAuth->GetApiId() <= 0 || configAuth->GetApiHash().empty()) {
        printf("You can generate api credentials on 'https://my.telegram.org'\n");
        configAuth->SetApiId(std::stoi(tdlpp::utils::getpass("  Enter api id: ")));
        configAuth->SetApiHash(tdlpp::utils::getpass("  Enter api hash: "));
    }


    auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
    parameters->database_directory_ = configAuth->GetTdDatabaseDir();
    parameters->api_id_ = configAuth->GetApiId();
    parameters->api_hash_ = configAuth->GetApiHash();
    parameters->system_language_code_ = configAuth->GetTdLanguageCode();
    parameters->device_model_ = configAuth->GetTdDeviceModel();
    parameters->application_version_ = applicationVersion;

    parameters->use_chat_info_database_ = true;
    parameters->use_message_database_ = true;
    parameters->use_file_database_ = true;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::setTdlibParameters>(std::move(parameters)),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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


void spy::auth::SpyAuth::OnAuthStateClosed() {
    TDLPP_LOG_ERROR(" ");
    authorized = false;
    authLock.notify_all();
}

void spy::auth::SpyAuth::OnAuthStateWaitCode() {
    TDLPP_LOG_DEBUG(" ");

    std::cout << "  Enter authentication code: " << std::flush;
    std::string code;
    std::cin >> code;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkAuthenticationCode>(code),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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

void spy::auth::SpyAuth::OnAuthStateRegistration() {
    TDLPP_LOG_DEBUG(" ");

    std::string first_name;
    std::string last_name;
    std::cout << "  Enter your first name: " << std::flush;
    std::cin >> first_name;
    std::cout << "  Enter your last name: " << std::flush;
    std::cin >> last_name;

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::registerUser>(first_name, last_name),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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

void spy::auth::SpyAuth::OnAuthStateWaitPassword() {
    TDLPP_LOG_DEBUG(" ");

    std::string password = tdlpp::utils::getpass("  Enter authentication password: ");

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkAuthenticationPassword>(password),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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

void spy::auth::SpyAuth::OnAuthStateWaitPhoneNumber() {
    TDLPP_LOG_DEBUG(" ");

    std::cout << "  Enter phone number: ";
    std::string phone_number;
    std::getline(std::cin, phone_number);


    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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

void spy::auth::SpyAuth::OnAuthStateWaitEncryptionKey() {
    TDLPP_LOG_DEBUG(" ");

    handler_->ExecuteAsync(td::td_api::make_object<td::td_api::checkDatabaseEncryptionKey>(""),
        [this](tdlpp::SharedObjectPtr<td::td_api::Object> object) {
            td::td_api::downcast_call(*object, tdlpp::overloaded(
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
