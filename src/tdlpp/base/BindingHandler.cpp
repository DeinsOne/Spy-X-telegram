
#include <tdlpp/base/BindongHandler.hpp>

std::shared_ptr<tdlpp::base::BindingHandler> tdlpp::base::BindingHandler::create() {
    return std::make_shared<tdlpp::base::BindingHandler>();
}

void tdlpp::base::BindingHandler::Handle(const std::uint64_t& requestId, const SharedObjectPtr<td::td_api::Object>& object) {
    TDLPP_LOG_DEBUG("rid:%ld %s", requestId, TDLPP_TD_ID_NAME(object->get_id()));
    std::lock_guard<std::mutex> _lock(promisesLock);

    auto promise = promises.find(requestId);
    if (promise != promises.end()) {
        promise->second->SetResponse(object);
        promises.erase(promise);
        return;
    }
}
