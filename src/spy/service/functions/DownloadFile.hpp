
#ifndef spy_DownloadFileFunction
#define spy_DownloadFileFunction

#include <spy/service/functions/IFunction.h>

#include <spy/service/controller/ControllersHandler.hpp>

#include <tdlpp/base/Handler.hpp>
#include <td/telegram/td_api.hpp>

namespace spy { namespace service { namespace functions {

    class DownloadFile : IFunction {
    public:
        DownloadFile(
            const std::shared_ptr<spy::service::controller::ControllersHandler>& controllersHandler,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            td::td_api::file& file,
            const std::size_t priority
        )
            : services(controllersHandler), handler_(handler), file_(file), priority_(priority)
        {}

    public:
        virtual void Execute() override;

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> handler_;
        std::shared_ptr<spy::service::controller::ControllersHandler> services;
        td::td_api::file& file_;
        std::size_t priority_;

    };

} } } // namespace spy { namespace service { namespace functions {

#endif // spy_DownloadFileFunction
