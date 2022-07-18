
#ifndef spy_SendMessageFunction
#define spy_SendMessageFunction

#include <spy/service/functions/IFunction.h>
#include <tdlpp/base/Handler.hpp>

namespace spy { namespace service { namespace functions {

    enum class SendMessageParseMode {
        ParseModeMarkdown = 0,
        ParseModeHtml
    };

    class SendMessage : IFunction {
    public:
        SendMessage(
            const std::string& message,
            const std::int64_t& chatId,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler,
            const SendMessageParseMode parseMode = SendMessageParseMode::ParseModeMarkdown
        )
            : message_(message), chatId_(chatId), handler_(handler), parseMode_(parseMode)
        {}

    public:
        virtual void Execute() override {
            switch ((int)parseMode_) {
                case (int)SendMessageParseMode::ParseModeMarkdown: return SendMarkdown();
                case (int)SendMessageParseMode::ParseModeHtml: return SendHtml();
            }
        }

    private:
        void SendMarkdown();
        void SendHtml();

    protected:
        std::shared_ptr<tdlpp::base::TdlppHandler> handler_;
        std::string message_;
        std::int64_t chatId_;
        SendMessageParseMode parseMode_;
    };

} } } // namespace spy { namespace service { namespace functions {

#endif // spy_SendMessageFunction
