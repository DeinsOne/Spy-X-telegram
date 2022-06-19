
#ifndef spy_DatabaseComponent
#define spy_DatabaseComponent

#include <spy/db/MessagesDb.hpp>
#include <spy/db/SessionsDb.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/auth/ConfigAuth.hpp>

namespace spy { namespace db {

    class DatabaseComponents {
    public:
        /**
         * Create messages database client
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<MessagesDb>, messagesDb)([] {
            /* Create database-specific ConnectionProvider */
            auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>("spy-data.sqlite");

            /* Create database-specific ConnectionPool */
            auto connection = oatpp::sqlite::ConnectionPool::createShared(
                connectionProvider,
                10 /* max-connections */,
                std::chrono::seconds(5) /* connection TTL */
            );

            /* Create database-specific Executor */
            auto executor = std::make_shared<oatpp::sqlite::Executor>(connection);

            /* Create MyClient database client */
            return std::make_shared<MessagesDb>(executor);
        }());


        /**
         * Create sessions database client
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<SessionsDb>, sessionsDb)([] {
            auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>("spy-sessions.sqlite");

            auto connection = oatpp::sqlite::ConnectionPool::createShared(
                connectionProvider,
                10 /* max-connections */,
                std::chrono::seconds(5) /* connection TTL */
            );

            auto executor = std::make_shared<oatpp::sqlite::Executor>(connection);

            return std::make_shared<SessionsDb>(executor);
        }());

    };

} } // namespace spy { namespace db {


#endif // spy_DatabaseComponent
