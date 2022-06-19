
#include <tdlpp/base/Handler.hpp>

std::shared_ptr<tdlpp::base::TdlppHandler> tdlpp::base::TdlppHandler::create(const std::shared_ptr<auth::IAuth>& auth_) {
    auto handler = std::make_shared<tdlpp::base::TdlppHandler>(auth_);
    handler->this_ = handler;
    handler->auth_->SetHandler(handler);
    return handler;
}

tdlpp::base::TdlppHandler::TdlppHandler(const std::shared_ptr<auth::IAuth>& auth__) : auth_(auth__), destroy(false) {
    TDLPP_LOG_VERBOSE("tdlpp::base::TdlppHandler::constructor");
    updatesHandler = UpdateCallbacksHandler::create();
    binding = BindingHandler::create();
    if (this_)
        auth_->SetHandler(this_);

    SetCallback<td::td_api::updateAuthorizationState>(true, [&](td::td_api::updateAuthorizationState& state) {
        auth_->OnAuthStateUpdate(std::move(state));
    });

    auto func = [&]() {
        TDLPP_LOG_DEBUG("Create worker thread %d", std::this_thread::get_id());
        while (!destroy) {
            objectsQueueMutex.lock();
            if (objectsQueue.size()) {
                auto object = objectsQueue.front();
                objectsQueue.pop();
                objectsQueueMutex.unlock();
                Handle(object.first, object.second);
            }
            else {
                objectsQueueMutex.unlock();
                thread_local std::mutex mtx;
                thread_local std::unique_lock<std::mutex> unique(mtx);
                queueNotifier.wait_for(unique, std::chrono::seconds(5));
            }
        }

        destroy = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        TDLPP_LOG_DEBUG("Destroy worker thread %d", std::this_thread::get_id());
        destroyLock.notify_all();
    };

    worker = std::thread(func);
    worker.detach();
}

tdlpp::base::TdlppHandler::~TdlppHandler() {
    destroy = true;
    queueNotifier.notify_all();

    std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);
    destroyLock.wait(lock, [&]() { return !destroy; });
}


void tdlpp::base::TdlppHandler::Handle(const std::uint64_t& requestId, SharedObjectPtr<td::td_api::Object> object) {
    TDLPP_LOG_DEBUG("rid:%ld %s", requestId, TDLPP_TD_ID_NAME(object->get_id()));

    if (requestId == 0) {
        updatesHandler->Handle(object);
    }

    binding->Handle(requestId, object);
}

void tdlpp::base::TdlppHandler::PushToQueue(const std::uint64_t& requestId, UniqueObjectPtr<td::td_api::Object> object) {
    TDLPP_LOG_DEBUG("rid:%ld %s", requestId, TDLPP_TD_ID_NAME(object->get_id()));
    std::shared_ptr<td::td_api::Object> objectPtr(std::move(object).release());
    std::lock_guard<std::mutex> lock(objectsQueueMutex);
    objectsQueue.push(std::pair<std::uint64_t,SharedObjectPtr<td::td_api::Object>>(requestId, objectPtr));
    queueNotifier.notify_all();
}
