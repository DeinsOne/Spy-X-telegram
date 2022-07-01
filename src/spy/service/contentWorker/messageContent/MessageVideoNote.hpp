
#ifndef spy_MessageVideoNoteWorker
#define spy_MessageVideoNoteWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>
#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessageVideoNote : public IContentWorker {
    public:
        MessageVideoNote(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVideoNote& message) {
                    auto result = messagesDb->executeQuery(
                        "INSERT INTO messageVideoNote (id, chat_id, version_, is_secret_, is_viewed, file_id_, duration, length) VALUES " \
                        "(:id, :chat_id, :version, :secret, :viewed, :file_id, :d, :l);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "secret", oatpp::Boolean(message.is_secret_) },
                            { "viewed", oatpp::Boolean(message.is_viewed_) },
                            { "file_id", oatpp::String(message.video_note_->video_->remote_->unique_id_) },
                            { "d", oatpp::Int32((int)message.video_note_->duration_) },
                            { "l", oatpp::Int32((int)message.video_note_->length_) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessageVideoNote:AddToDatabase Error: %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessageVideoNote:AddToDatabase Accepted bad content, messageVideoNote expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVideoNote& message) {
                    auto priority = message.is_secret_ ? 1 : 4;
                    spy::service::functions::DownloadFile download(controllers, handler, *message.video_note_->video_, priority);
                    download.Execute();
                },
                [](auto&) {
                    SPY_LOGF("MessageVideoNote:DownloadContent Accepted bad content, messageVideoNote expected");
                }
            ));
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessageVideoNoteWorker
