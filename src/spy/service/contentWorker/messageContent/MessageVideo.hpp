
#ifndef spy_MessageVideoWorker
#define spy_MessageVideoWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>
#include <spy/dto/messageContent/MessageVideo.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>
#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessageVideo : public IContentWorker {
    public:
        MessageVideo(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVideo& message) {
                    spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
                    parser.Execute();

                    auto result = messagesDb->executeQuery(
                        "INSERT INTO messageVideo (id, chat_id, version_, caption_, is_secret_, file_id_, duration, width, height, mime_type) VALUES " \
                        "(:id, :chat_id, :version, :caption, :secret, :file_id, :d, :w, :h, :mime);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "caption", oatpp::String(parser.GetMarkdown()) },
                            { "secret", oatpp::Boolean(message.is_secret_) },
                            { "file_id", oatpp::String(message.video_->video_->remote_->unique_id_) },
                            { "d", oatpp::Int32((int)message.video_->duration_) },
                            { "w", oatpp::Int32((int)message.video_->width_) },
                            { "h", oatpp::Int32((int)message.video_->height_) },
                            { "mime", oatpp::String(message.video_->mime_type_) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessageVideo:AddToDatabase Error: %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessageVideo:AddToDatabase Accepted bad content, messageVideo expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messageVideo& message) {
                    auto priority = message.is_secret_ ? 1 : 20;
                    spy::service::functions::DownloadFile download(controllers, handler, *message.video_->video_, priority);
                    download.Execute();
                },
                [](auto&) {
                    SPY_LOGF("MessageVideo:DownloadContent Accepted bad content, messageVideo expected");
                }
            ));
        }

        virtual oatpp::Any GetFromDatabase(const std::int32_t& version) override {
            return content::GetFromDatabase<spy::dto::messageContent::MessageVideoDto>(messagesDb, "messageVideo", chat_id, message_id, version);
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessageVideoWorker
