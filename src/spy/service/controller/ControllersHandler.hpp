
#ifndef spy_ControllersHandler
#define spy_ControllersHandler

#include <spy/service/controller/BaseController.hpp>
#include <unordered_map>

#include <spy/service/controller/ChatsController/ChatsController.hpp>
#include <spy/service/controller/SettingsController/SpySettingsController.hpp>
#include <spy/service/controller/NewContentController/NewContentController.hpp>
#include <spy/service/controller/DeletedContentController/DeletedContentController.hpp>
#include <spy/service/controller/DeletedContentChatController/DeletedContentChatController.hpp>

namespace spy { namespace service { namespace controller {

    class ControllersHandler {
    public:
        static std::shared_ptr<ControllersHandler> CreateShared(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) {
            auto controllerHandler = std::make_shared<ControllersHandler>(handler);
            controllerHandler->this_ = controllerHandler;
            return controllerHandler;
        }

        ControllersHandler(const std::shared_ptr<tdlpp::base::TdlppHandler>& handler) : tdlppHandler(handler) {
        }

    public:
        template<typename TController, typename... TArgs>
        void AddController(TArgs... args) {
            auto controller = std::make_shared<TController>(args...);
            controller->SetSpyService(this_);
            controller->RegisterUpdates(tdlppHandler);
            controller->Initialize(tdlppHandler);
            controllers.insert({TController::ID, controller});
        }

        template<typename TController>
        TController* GetController() {
            auto controller = controllers.at(TController::ID);
            if (!controller) return nullptr;
            return (TController*)controller.get();
        }

    private:
        std::unordered_map<std::int32_t, std::shared_ptr<BaseController>> controllers;
        std::shared_ptr<tdlpp::base::TdlppHandler> tdlppHandler;
        std::shared_ptr<ControllersHandler> this_;
    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_ControllersHandler
