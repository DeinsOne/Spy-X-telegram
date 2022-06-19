
#ifndef tdlpp_BindingHandler
#define tdlpp_BindingHandler

#include <tdlpp/core.hpp>
#include <tdlpp/base/ResponsePromise.hpp>
#include <unordered_map>

namespace tdlpp { namespace base {

    // Binds responses with appropriate promises
    class BindingHandler : public TdlppBase {
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<BindingHandler> New instance of 'BindingHandler'
         */
        static std::shared_ptr<BindingHandler> create();

    public:
        /**
         * @brief Binds object with appropriate promsise. Removes promise from hanling list
         * 
         * @param requestId Id of response will be handled
         * @param object Object to be handled
         */
        void Handle(const std::uint64_t& requestId, const SharedObjectPtr<td::td_api::Object>& object);

        /**
         * @brief Add promise to handling list
         * 
         * @tparam TdFunction Inherited from td::td_api::Function
         * @param promise Promise to be handled
         */
        template<typename TdFunction>
        void Listen(const std::shared_ptr<ResponsePromise<TdFunction>>& promise) {
            TDLPP_LOG_DEBUG("rid:%ld %s", promise->GetRID(), TDLPP_TD_ID_NAME(TdFunction::ID));
            std::lock_guard<std::mutex> _lock(promisesLock);
            promises[promise->GetRID()] = promise;
        }

    private:
        // Promises contains ordered map of request ids and appropriate promises to them: std::unordered_map<rid; promise> 
        std::unordered_map<std::uint64_t, SharedObjectPtr<IResponsePromise>> promises;
        std::mutex promisesLock;
    };

} // namespace base
} // namespace tdlpp


#endif // tdlpp_BindingHandler
