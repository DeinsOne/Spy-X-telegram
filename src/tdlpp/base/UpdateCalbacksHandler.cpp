
#include <tdlpp/base/UpdateCalbacksHandler.hpp>

std::shared_ptr<tdlpp::base::UpdateCallbacksHandler> tdlpp::base::UpdateCallbacksHandler::create() {
    auto handler = std::make_shared<tdlpp::base::UpdateCallbacksHandler>();
    handler->this_ = handler;
    return handler;
}

tdlpp::base::UpdateCallbacksHandler::UpdateCallbacksHandler() {
    TDLPP_LOG_VERBOSE("tdlpp::router::UpdateCallbacksHandler::constructor");
}
