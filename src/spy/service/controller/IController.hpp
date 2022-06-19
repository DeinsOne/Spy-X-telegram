
#ifndef spy_IController
#define spy_IController

#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace controller {

    class IController {
    public:
        virtual void Initialize(const std::shared_ptr<tdlpp::base::TdlppHandler>&) = 0;
        virtual void RegisterUpdates(const std::shared_ptr<tdlpp::base::TdlppHandler>&) = 0;
    };

} } } // namespace spy { namespace service { namespace controller {


#endif // spy_IController
