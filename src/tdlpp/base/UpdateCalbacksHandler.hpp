
#ifndef tdlpp_UpdateCallbacksHandler
#define tdlpp_UpdateCallbacksHandler

#include <tdlpp/core.hpp>
#include <memory>

#include <functional>

#include <unordered_map>
#include <vector>

namespace tdlpp { namespace base {

    // Update handler processes update with registred callbacks
    class UpdateCallbacksHandler : public TdlppBase {
    public:
        /**
         * @brief A factory method
         * 
         * @return std::shared_ptr<UpdateCallbacksHandler> New instance of 'UpdateCallbacksHandler'
         */
        static std::shared_ptr<UpdateCallbacksHandler> create();

    public:
        UpdateCallbacksHandler();

        /**
         * @brief Add a callback to the list of callbacks
         * 
         * @tparam TdUpdate Inherited from td::td_api::Update
         * @param func Callback function
         * @return std::shared_ptr<UpdateCallbacksHandler> Return pointer to this. Provides chaining
         */
        template<typename TdUpdate>
        std::shared_ptr<UpdateCallbacksHandler> Listen(const std::function<void(TdUpdate&)>& func) {
            TDLPP_LOG_DEBUG("%s", TDLPP_TD_ID_NAME(TdUpdate::ID));
            callbacks[TdUpdate::ID].emplace_back(new std::function<void(TdUpdate&)>(func));
            return this_;
        }

        /**
         * @brief Removing all callbacks for a specific update type
         * 
         * @tparam TdUpdate Inherited from td::td_api::Update
         */
        template<typename TdUpdate>
        void RemoveCallbacks() {
            TDLPP_LOG_DEBUG("%s", TDLPP_TD_ID_NAME(TdUpdate::ID));
            callbacks.erase(TdUpdate::ID);
        }

        /**
         * @brief Handle update object throught registred callbacks
         * 
         * @tparam TdUpdate Inherited from td::td_api::Update
         * @param objectPtr Object instance to be handled 
         */
        template<typename TdUpdate>
        void Handle(const SharedObjectPtr<TdUpdate>& objectPtr) {
            TDLPP_LOG_DEBUG("%s", TDLPP_TD_ID_NAME(objectPtr->get_id()));

            auto current = callbacks.find(objectPtr->get_id());

            if (current != callbacks.end()) {
                td::td_api::downcast_call(*objectPtr, overloaded([&](auto& casted) {
                    for (int i = 0; i < current->second.size(); i++) {
                        auto callback = reinterpret_cast<std::function<void(decltype(casted)&)>*>(current->second.at(i));
                        if (callback) {
                            TDLPP_LOG_DEBUG("callback[%d]", i);
                            callback->operator()(casted);
                        }
                    }
                }));
            }
        }

    private:
        std::shared_ptr<UpdateCallbacksHandler> this_;
        std::unordered_map<std::int32_t,std::vector<void*>> callbacks;
    };

} // namespace base
} // namespace tdlpp

#endif // tdlpp_UpdateCallbacksHandler
