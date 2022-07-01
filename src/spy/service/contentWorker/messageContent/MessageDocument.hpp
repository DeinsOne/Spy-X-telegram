
#ifndef spy_MessageDocumentWorker
#define spy_MessageDocumentWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>
#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessageDocument : public IContentWorker {
    public:
        MessageDocument(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageDocument& message) {
                    spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
                    parser.Execute();

                    auto result = messagesDb->executeQuery(
                        "INSERT INTO messageDocument (id, chat_id, version_, caption, file_id_, mime_type) VALUES " \
                        "(:id, :chat_id, :version, :caption, :file_id, :mime);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "caption", oatpp::String(parser.GetMarkdown()) },
                            { "file_id", oatpp::String(message.document_->document_->remote_->unique_id_) },
                            { "mime", oatpp::String(message.document_->mime_type_) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessageDocument:AddToDatabase Error: %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessageDocument:AddToDatabase Accepted bad content, messageDocument expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageDocument& message) {
                    spy::service::functions::DownloadFile download(controllers, handler, *message.document_->document_, 20);
                    download.Execute();
                },
                [](auto&) {
                    SPY_LOGF("MessageDocument:DownloadContent Accepted bad content, messageDocument expected");
                }
            ));
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessageDocumentWorker
