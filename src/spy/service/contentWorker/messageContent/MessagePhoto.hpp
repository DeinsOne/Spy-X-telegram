
#ifndef spy_MessagePhotoWorker
#define spy_MessagePhotoWorker

#include <spy/service/contentWorker/ContentWorker.hpp>
#include <oatpp/core/macro/component.hpp>
#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include <spy/service/functions/Utils/FormattedTextToMarkdown.hpp>
#include <spy/service/functions/DownloadFile/DownloadFile.hpp>

#include <spy/utils/Logger/SpyLog.h>

namespace spy { namespace service { namespace content {

    class MessagePhoto : public IContentWorker {
    public:
        MessagePhoto(const std::int64_t& message_id, const std::int64_t& chat_id)
         : message_id(message_id), chat_id(chat_id)
        {
        }

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messagePhoto& message) {
                    spy::service::functions::FormattedTextToMarkdown parser(*message.caption_);
                    parser.Execute();

                    auto result = messagesDb->executeQuery(
                        "INSERT INTO messagePhoto (id, chat_id, version_, caption_, is_secret_, file_id_, width, height) VALUES " \
                        "(:id, :chat_id, :version, :caption, :secret, :file_id, :w, :h);",
                        {
                            { "id", oatpp::Int64(message_id) },
                            { "chat_id", oatpp::Int64(chat_id) },
                            { "version", oatpp::Int64(version) },
                            { "caption", oatpp::String(parser.GetMarkdown()) },
                            { "secret", oatpp::Boolean(message.is_secret_) },
                            { "file_id", oatpp::String(message.photo_->sizes_.back()->photo_->remote_->unique_id_) },
                            { "w", oatpp::Int32((int)message.photo_->sizes_.back()->width_) },
                            { "h", oatpp::Int32((int)message.photo_->sizes_.back()->height_) }
                        }
                    );

                    if (!result->isSuccess()) {
                        SPY_LOGE("MessagePhoto:AddToDatabase Error: %s", result->getErrorMessage()->c_str());
                    }
                },
                [](auto&) {
                    SPY_LOGF("MessagePhoto:AddToDatabase Accepted bad content, messagePhoto expected");
                }
            ));
        }

        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override {
            td::td_api::downcast_call(content, tdlpp::overloaded(
                [&](td::td_api::messagePhoto& message) {
                    auto priority = message.is_secret_ ? 1 : 5;
                    spy::service::functions::DownloadFile download(controllers, handler, *message.photo_->sizes_.back()->photo_, priority);
                    download.Execute();
                },
                [](auto&) {
                    SPY_LOGF("MessagePhoto:DownloadContent Accepted bad content, messagePhoto expected");
                }
            ));
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

        std::int64_t message_id;
        std::int64_t chat_id;
    };

} } }


#endif // spy_MessagePhotoWorker