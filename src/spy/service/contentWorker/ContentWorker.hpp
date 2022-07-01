
#ifndef spy_ContentWorker
#define spy_ContentWorker

#include <memory>

#include <tdlpp/base/Handler.hpp>
#include <spy/service/controller/ControllersHandler.hpp>

namespace spy { namespace service { namespace content {

    class IContentWorker {
    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) = 0;
        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) = 0;
    };


    class ContentWorkerFactory : public IContentWorker {
    public:
        ContentWorkerFactory(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id);

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override;
        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override;

    public:
        static std::shared_ptr<IContentWorker> matchContent(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id);

    private:
        std::shared_ptr<IContentWorker> contentWorker;
        std::int64_t messageId;
    };

} } }

#endif // spy_ContentWorker
