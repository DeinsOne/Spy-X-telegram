
#include <spy/service/SpyService.hpp>

spy::service::SpyService::SpyService(
    const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
    const std::shared_ptr<controller::ControllersHandler>& controllersHandler
)
    : tdlppHandler(handler), controllersHandler(controllersHandler)
{
}

std::shared_ptr<spy::service::SpyService> spy::service::SpyService::CreateShared(
    const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
    const std::shared_ptr<controller::ControllersHandler>& controllersHandler
) {
    return std::make_shared<SpyService>(handler, controllersHandler);
}


