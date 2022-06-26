
#ifndef spy_SendMessageFunction
#define spy_SendMessageFunction

#include <spy/service/functions/IFunction.h>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace functions {

    class SendMessage : IFunction {
    public:
        SendMessage(
            const std::string& message,
            const std::int64_t& chatId,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        )
            : message_(message), chatId_(chatId), handler_(handler)
        {}

    public:
        virtual void Execute() override;

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> handler_;
        std::string message_;
        std::int64_t chatId_;
    };

} } } // namespace spy { namespace service { namespace functions {

#endif // spy_SendMessageFunction
