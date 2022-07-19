
#ifndef spy_MessageVoiceNoteWorker
#define spy_MessageVoiceNoteWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <spy/dto/messageContent/MessageVoiceNote.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>
#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessageVoiceNote : public IContentWorker {
    public:
        MessageVoiceNote(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVoiceNote& message) {
                    spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
                    parser.Execute();

                    auto result = messagesDb->executeQuery(
                        "INSERT INTO messageVoiceNote (id, chat_id, version_, is_listened, file_id_, duration, mime_type) VALUES " \
                        "(:id, :chat_id, :version, :listened, :file_id, :d, :mime);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "listened", oatpp::Boolean(message.is_listened_) },
                            { "file_id", oatpp::String(message.voice_note_->voice_->remote_->unique_id_) },
                            { "d", oatpp::Int32((int)message.voice_note_->duration_) },
                            { "mime", oatpp::String(message.voice_note_->mime_type_) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessageVoiceNote:AddToDatabase Error: %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessageVoiceNote:AddToDatabase Accepted bad content, messageVoiceNote expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVoiceNote& message) {
                    spy::service::functions::DownloadFile download(controllers, handler, *message.voice_note_->voice_, 10);
                    download.Execute();
                },
                [](auto&) {
                    SPY_LOGF("MessageVoiceNote:DownloadContent Accepted bad content, messageVoiceNote expected");
                }
            ));
        }

        virtual oatpp::Any GetFromDatabase(const std::int32_t& version) override {
            return content::GetFromDatabase<spy::dto::messageContent::MessageVoiceNoteDto>(messagesDb, "messageVoiceNote", chat_id, message_id, version);
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessageVoiceNoteWorker
