
#ifndef spy_DatabaseComponent
#define spy_DatabaseComponent

#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <spy/db/SessionsDatabase/SessionsDatabase.hpp>
#include <spy/db/ChatsDatabase/ChatsDatabase.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/auth/ConfigAuth.hpp>

namespace spy { namespace db {

    class DatabaseComponents {
    public:

        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dataDbConnectionProvider)("SpyDataDatabase", [] {
            auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(SPY_DATA_FOLDER "spy-data.sqlite");

            return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                10 /* max-connections */,
                std::chrono::seconds(5) /* connection TTL */
            );
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<MessagesDatabase>, messagesDb)([] {
            OATPP_COMPONENT(
                std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                connectionProvider,
                "SpyDataDatabase"
            );

            /* Create database-specific Executor */
            auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

            /* Create MyClient database client */
            return std::make_shared<MessagesDatabase>(executor);
        }());



        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, chatsDbConnectionProvider)("ChatsDatabase", [] {
            auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(SPY_DATA_FOLDER "spy-chats.sqlite");

            return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                2 /* max-connections */,
                std::chrono::seconds(5) /* connection TTL */
            );
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<ChatsDatabase>, chatsDb)([] {
            OATPP_COMPONENT(
                std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                connectionProvider,
                "ChatsDatabase"
            );

            auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

            return std::make_shared<ChatsDatabase>(executor);
        }());



        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, sessionsDbConnectionProvider)("SessionsDatabase", [] {
            auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(SPY_DATA_FOLDER "spy-sessions.sqlite");

            return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                2,
                std::chrono::seconds(5)
            );
        }());

        OATPP_CREATE_COMPONENT(std::shared_ptr<SessionsDatabase>, sessionsDb)([] {
            OATPP_COMPONENT(
                std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>,
                connectionProvider,
                "SessionsDatabase"
            );

            auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

            return std::make_shared<SessionsDatabase>(executor);
        }());

    };

} } // namespace spy { namespace db {


#endif // spy_DatabaseComponent
