
#ifndef spy_SpyComponent
#define spy_SpyComponent

#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <spy/db/DatabaseComponents.hpp>
#include <spy/controller/ErrorHandler.hpp>

#include <spy/utils/CmdParserSingleton.hpp>

namespace spy {

    class SpyComponents {
    public:
        /**
         * Create ObjectMapper component to serialize/deserialize DTOs in Controller's API
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
            auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
            objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
            objectMapper->getSerializer()->getConfig()->includeNullFields = false;
            objectMapper->getSerializer()->getConfig()->useBeautifier = false;
            return objectMapper;
        }());

        /**
         *  Create ConnectionProvider component which listens on the port
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
            return oatpp::network::tcp::server::ConnectionProvider::createShared({
                "0.0.0.0",
                (v_uint16)spy::utils::CmdParserSingleton::Get()->GetArgument<int>("port"),
                oatpp::network::Address::IP_4
            });
        }());

        /**
         *  Create Router component
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
            return oatpp::web::server::HttpRouter::createShared();
        }());

        /**
         *  Create ConnectionHandler component which uses Router component to route requests
         */
        OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)("http", [] {
            OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
            OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper); // get ObjectMapper component

            auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
            connectionHandler->setErrorHandler(std::make_shared<controller::ErrorHandler>(objectMapper));
            return connectionHandler;
        }());

    };

} // namespace spy


#endif // spy_SpyComponent
