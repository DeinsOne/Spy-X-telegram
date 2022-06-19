
#ifndef spy_BaseController
#define spy_BaseController

#include <spy/service/controller/IController.hpp>

namespace spy { namespace service { namespace controller {

    class ControllersHandler;

    class BaseController : public IController {
    public:
        virtual void SetSpyService(const std::shared_ptr<ControllersHandler>& service) {
            this->service = service;
        }

        virtual std::int32_t GetControllerId() = 0;

    protected:
        std::shared_ptr<ControllersHandler> service;
        bool initialized{false};
    };

} } } // namespace spy { namespace service { namespace controller {

#endif // spy_BaseController
