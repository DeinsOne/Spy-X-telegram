
#ifndef spy_MessageTextWorker
#define spy_MessageTextWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessageText : public IContentWorker {
    public:
        MessageText(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageText& message) {
                    spy::service::functions::FormattedTextToMarkdown parser(*message.text_);
                    parser.Execute();

                    auto result = messagesDb->executeQuery("INSERT INTO messageText (id, chat_id, version_, text_) VALUES " \
                        "(:id, :chat_id, :version, :text);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "text", oatpp::String(parser.GetMarkdown()) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessagesDatabase:addMessageContent(messageText): %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessageText:AddToDatabase Accepted bad content, messageText expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessageTextWorker
