
#ifndef tdlpp_ResponsePromise
#define tdlpp_ResponsePromise

#include <tdlpp/core.hpp>
#include <condition_variable>
#include <functional>

namespace tdlpp { namespace base {

    class BindingHandler;

    class IResponsePromise {
        friend BindingHandler;
    public:
        virtual std::uint64_t GetRID() = 0;
        virtual std::int32_t getTdTypeId() = 0;

        virtual std::shared_ptr<td::td_api::Object> GetResponse() = 0;

    private:
        virtual void SetResponse(SharedObjectPtr<td::td_api::Object> response_) = 0;

    };


    /**
     * @brief Promise associated with td function. Can be used to access async request's reponse
     * 
     * @tparam TdFunction Inherited from td::td_api::Function
     */
    template<typename TdFunction>
    class ResponsePromise : public IResponsePromise {
    public:
        /**
         * @brief A factory method
         * 
         * @param rid Request id of thrown request associated with this promise
         * 
         * @return std::shared_ptr<BindingHandler> New instance of 'BindingHandler'
         */
        static std::shared_ptr<ResponsePromise> create(const std::uint64_t& rid) {
            return std::make_shared<tdlpp::base::ResponsePromise<TdFunction>>(rid);
        }
        /**
         * @brief A factory method
         * 
         * @param rid Request id of thrown request associated with this promise
         * @param callback Calback to be called on response handling
         * 
         * @return std::shared_ptr<BindingHandler> New instance of 'BindingHandler'
         */
        static std::shared_ptr<ResponsePromise> create(const std::uint64_t& rid, const std::function<void(SharedObjectPtr<td::td_api::Object>)>& callback) {
            return std::make_shared<tdlpp::base::ResponsePromise<TdFunction>>(rid, callback);
        }

    public:
        ResponsePromise(const std::uint64_t& requestId) : rid(requestId) {
            TDLPP_LOG_VERBOSE("constructor");
        }
        ResponsePromise(const std::uint64_t& requestId, const std::function<void(SharedObjectPtr<td::td_api::Object>)>& callback_)
            : rid(requestId), callback(callback_) 
        {
            TDLPP_LOG_VERBOSE("constructor");
        }

        // Retruns associated request id
        virtual std::uint64_t GetRID() override { return rid; };

        // Retruns associated td function type id
        virtual std::int32_t getTdTypeId() override { return TdFunction::ID; };

        // Waiting for response to be set. Locks executing thread
        virtual std::shared_ptr<td::td_api::Object> GetResponse() override {
            if (response) return response;
            TDLPP_LOG_DEBUG("rid:%ld lock, wait response", rid);
            std::mutex _mtx;
            std::unique_lock<std::mutex> ulock(_mtx);
            lock.wait(ulock, [&] { return response != nullptr; });
            TDLPP_LOG_DEBUG("rid:%ld unlock", rid);
            return response;
        }

    private:
        // Setting the response and notifying the waiting thread.
        virtual void SetResponse(SharedObjectPtr<td::td_api::Object> response_) override {
            TDLPP_LOG_DEBUG("rid:%ld %s", rid, TDLPP_TD_ID_NAME(response_->get_id()));
            response = response_;
            if (callback) {
                TDLPP_LOG_DEBUG("%s trigger callback rid:%ld", TDLPP_TD_ID_NAME(response_->get_id()), rid);
                callback(response);
            }
            lock.notify_all();
        }

    private:
        std::uint64_t rid;
        std::shared_ptr<td::td_api::Object> response;

        std::function<void(SharedObjectPtr<td::td_api::Object>)> callback;
        std::condition_variable lock;
    };


} // namespace base
} // namespace tdlpp

#endif // tdlpp_ResponsePromise
