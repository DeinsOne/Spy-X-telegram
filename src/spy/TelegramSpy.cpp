
#include <tdlpp/tdlpp.hpp>

#include <oatpp/core/base/Environment.hpp>
#include <oatpp/network/Server.hpp>
#include <csignal>

#include <spy/auth/SpyAuth.hpp>
#include <spy/SpyComponents.hpp>
#include <spy/service/SpyService.hpp>
#include <spy/utils/CmdParserSingleton.hpp>
#include <spy/service/controller/ControllersHandler.hpp>

#include <spy/controller/StaticController.hpp>
#include <spy/controller/ChatsController/ChatsController.hpp>

#include <spy/utils/Logger/SpyLog.h>
#include <spy/version.h>

#if (CXX_FILESYSTEM_IS_EXPERIMENTAL == true)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #include <filesystem>
    namespace fs = std::filesystem;
#endif

int main(int argc, char** argv) {
    /* Parse command line arguments */
    spy::utils::CmdParserSingleton::Get()->ParseArguments(argc, argv);

    SPY_LOG_INIT();
    SPY_LOGI("Spy X Telegram version " SPY_VERSION_STRING "\n");

    /* Initializing oatpp environment */
    oatpp::base::Environment::init();


    /* Initializing tdlpp components */
    auto spyAuth = spy::auth::SpyAuth::create();

    auto tdlppHandler = tdlpp::base::TdlppHandler::create(spyAuth);
    auto tdlppLongPoll = tdlpp::poll::LongPoll::create(tdlppHandler);
    auto tdlppController = tdlppLongPoll->ExecuteAsync();

    /* Waiting for user to be logged in */
    if (!spyAuth->IsAuthorized()) {
        SPY_LOGD("Tdlpp:WaitAuthorized");
        spyAuth->WaitAuthorized();

        if (spyAuth->GetRetriesCount() >= TDLPP_MAX_AUTH_RETRIES) {
            return 1;
        }
    }


    /* Create spy data folder */
    fs::create_directories(SPY_DATA_FOLDER);

    /* Create database components */
    spy::db::DatabaseComponents dbcomponents;

    /* Initialize spy service relying on DatabaseComponents */
    auto controllersHandler = spy::service::controller::ControllersHandler::CreateShared(tdlppHandler);
    controllersHandler->AddController<spy::service::controller::SpySettingsController>();
    controllersHandler->AddController<spy::service::controller::ChatsController>();
    controllersHandler->AddController<spy::service::controller::NewContentController>();
    controllersHandler->AddController<spy::service::controller::DeletedContentChatController>();
    controllersHandler->AddController<spy::service::controller::DeletedContentController>();

    auto spyService = spy::service::SpyService::CreateShared(tdlppHandler, controllersHandler);

    /* Check command line args to enable/disable rest server */
    if (spy::utils::CmdParserSingleton::Get()->GetArgument<bool>("rest")) {
        /* Initializing oatpp components */
        spy::SpyComponents components;

        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

        /* Add endpoints */
        router->addController(spy::controller::StaticController::createShared());
        router->addController(spy::controller::ChatsController::createShared());

        /* Initialize server */
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

        oatpp::network::Server server(
            connectionProvider,
            connectionHandler
        );

        /* Print some info */
        spy::utils::CmdParserSingleton::Get()->GetArgument<std::string>("log_level") == "debug" ? printf("\n\n") : printf("\n");
        SPY_LOGI("Rest server:Running on http://localhost:%d", spy::utils::CmdParserSingleton::Get()->GetArgument<int>("port"));
        SPY_LOGI("Rest server:Endpoints on http://localhost:%d/swagger/ui\n", spy::utils::CmdParserSingleton::Get()->GetArgument<int>("port"));

        /* Start rest server */
        server.run();
    }
    else {
        std::mutex mtx;
        std::unique_lock<std::mutex> lock(mtx);
        std::condition_variable condition;
        condition.wait(lock);
    }


    /* Destroying oatpp environment */
    oatpp::base::Environment::destroy();

    return 0;
}
