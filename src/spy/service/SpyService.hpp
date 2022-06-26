
#ifndef spy_Service
#define spy_Service

#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/service/controller/ControllersHandler.hpp>

namespace spy { namespace service {

    class SpyService {
    public:
        SpyService(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            const std::shared_ptr<controller::ControllersHandler>& controllersHandler
        );

        static std::shared_ptr<SpyService> CreateShared(
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            const std::shared_ptr<controller::ControllersHandler>& controllersHandler
        );

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::shared_ptr<tdlpp::base::TdlppHandler> tdlppHandler;
        std::shared_ptr<controller::ControllersHandler> controllersHandler;

    };

} } // namespace spy { namespace { service


#endif // spy_Service
