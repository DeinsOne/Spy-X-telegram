
#ifndef tdlpp_Handler
#define tdlpp_Handler

#include <tdlpp/auth/IAuth.hpp>
#include <tdlpp/base/UpdateCalbacksHandler.hpp>
#include <tdlpp/router/Router.hpp>
#include <tdlpp/base/BindongHandler.hpp>
#include <functional>
#include <thread>
#include <queue>

namespace tdlpp { namespace poll {
    class LongPoll;
    class PollManager;
} // namespace poll
} // namespace tdlpp

namespace tdlpp { namespace base {

    // Controles processing of incoming responses
    class TdlppHandler : public TdlppBase {
        friend poll::PollManager;
        friend poll::LongPoll;
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<TdlppHandler> New instance of 'TdlppHandler'
         */
        static std::shared_ptr<TdlppHandler> create(const std::shared_ptr<auth::IAuth>& auth_);

    public:
        TdlppHandler(const std::shared_ptr<auth::IAuth>& auth__);

        ~TdlppHandler();

        /**
         * @brief Set a callback for a specific td update
         * 
         * @tparam TdUpdate Inherited from td::td_api::Update
         * @param func Callback function
         * @param overwrite Controles erasing all other update callbacks
         * @return std::shared_ptr<TdlppHandler> Pointer to this. Provides chaining
         */
        template<typename TdUpdate>
        std::shared_ptr<TdlppHandler> SetCallback(const bool& overwrite, const std::function<void(TdUpdate&)>& func) {
            TDLPP_LOG_DEBUG("%s", TDLPP_TD_ID_NAME(TdUpdate::ID));
            if (overwrite) updatesHandler->RemoveCallbacks<TdUpdate>();
            updatesHandler->Listen<TdUpdate>(func);
            return this_;
        }

        /**
         * @brief Execute td function asynchronously and return the promise
         * 
         * @tparam TdFunction Inherited from td::td_api::Function
         * @param function Td function object
         * @return std::shared_ptr<IResponsePromise> Promise to access response
         */
        template<typename TdFunction, typename... TArgs>
        std::shared_ptr<IResponsePromise> Execute(TArgs... args) {
            TDLPP_LOG_DEBUG("rid:%ld %s", router_->GetLastRequestId() + 1, TDLPP_TD_ID_NAME(TdFunction::ID));
            auto rid = router_->Send(td::td_api::make_object<TdFunction>(std::move(args)...));
            auto promise = ResponsePromise<TdFunction>::create(rid);
            binding->Listen(promise);
            return promise;
        }

        /**
         * @brief Execute td function asynchronously
         * 
         * @tparam TdFunction Inherited from td::td_api::Function
         * @param function Td function object pointer
         * @param handler Callback function to be called on response
         */
        template<typename TdFunction>
        void ExecuteAsync(UniqueObjectPtr<TdFunction> function, const std::function<void(SharedObjectPtr<td::td_api::Object>)>& handler = {} ) {
            TDLPP_LOG_DEBUG("rid:%ld %s", router_->GetLastRequestId() + 1, TDLPP_TD_ID_NAME(TdFunction::ID));
            auto rid = router_->Send(std::move(function));
            binding->Listen(ResponsePromise<TdFunction>::create(rid, handler));
        }

    private:
        std::shared_ptr<TdlppHandler> this_;
        std::shared_ptr<auth::IAuth> auth_;

        std::shared_ptr<router::Router> router_;
        std::shared_ptr<UpdateCallbacksHandler> updatesHandler;
        std::shared_ptr<BindingHandler> binding;

        std::thread worker;
        std::mutex objectsQueueMutex;
        std::condition_variable queueNotifier;
        std::queue<std::pair<std::uint64_t,SharedObjectPtr<td::td_api::Object>>> objectsQueue;
        bool destroy;
        std::condition_variable destroyLock;

        /**
         * @brief Pushes object to processing queue
         * 
         * @param requestId Respons's request id 
         * @param object Response object
         */
        void PushToQueue(const std::uint64_t& requestId, UniqueObjectPtr<td::td_api::Object> object);

        /**
         * @brief Handle incoming reponse
         * 
         * @param requestId Respons's request id 
         * @param object Response object
         */
        void Handle(const std::uint64_t& requestId, SharedObjectPtr<td::td_api::Object> object);

        void SetRouter(const std::shared_ptr<router::Router>& router__) { router_ = router__; }

    };

} // namespace base
} // namespace tdlpp

#endif // tdlpp_Handler
