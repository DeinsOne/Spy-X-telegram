
#ifndef spy_ContentWorker
#define spy_ContentWorker

#include <memory>

#include <tdlpp/base/Handler.hpp>
#include <spy/service/controller/ControllersHandler.hpp>

#include <oatpp/core/Types.hpp>
#include <spy/dto/MessageContent.hpp>

namespace spy { namespace service { namespace content {

    class IContentWorker {
    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) = 0;
        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) = 0;
        virtual oatpp::Any GetFromDatabase(const std::int32_t& version) = 0;
    };


    class ContentWorkerFactory : public IContentWorker {
    public:
        ContentWorkerFactory(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id);
        ContentWorkerFactory(const std::int32_t& td_type_id, const std::int64_t& message_id, const std::int64_t& chat_id);

    public:
        virtual void AddToDatabase(td::td_api::MessageContent& content, const std::int32_t& version) override;
        virtual void DownloadContent(
            td::td_api::MessageContent& content,
            const std::shared_ptr<controller::ControllersHandler>& controllers,
            const std::shared_ptr<tdlpp::base::TdlppHandler>& handler
        ) override;
        virtual oatpp::Any GetFromDatabase(const std::int32_t& version) override;

    public:
        static std::shared_ptr<IContentWorker> matchContent(td::td_api::MessageContent& content, const std::int64_t& message_id, const std::int64_t& chat_id);
        static std::shared_ptr<IContentWorker> matchContent(const std::int32_t& td_type_id, const std::int64_t& message_id, const std::int64_t& chat_id);

    private:
        std::shared_ptr<IContentWorker> contentWorker;
        std::int64_t messageId;
    };

    /**
     * @brief Reads message content from database
     * 
     * @tparam TContent represents dto content such as dto::messageContent::MessageTextDto
     * @param messagesDb Database with message contents
     * @param dbName Table name associated with this content
     * @param chatId Chat id
     * @param messageId Message id
     * @param version Message version
     * @return spy::dto::messageContent::MessageContentDto<TContent>
     */
    template<typename TContent>
    oatpp::Object<spy::dto::messageContent::MessageContentDto<TContent>> GetFromDatabase(
        const std::shared_ptr<db::MessagesDatabase>& messagesDb,
        const std::string& dbName,
        const std::int64_t& chatId,
        const std::int64_t& messageId,
        const std::int32_t& version)
    {
        auto result = messagesDb->executeQuery("SELECT * FROM " + oatpp::String(dbName) + " WHERE id=:id AND chat_id=:chat_id AND version_=:version;",
            {
                { "id", oatpp::Int64(messageId) },
                { "chat_id", oatpp::Int64(chatId) },
                { "version", oatpp::Int32(version) }
            }
        );

        if (!result->isSuccess()) {
            SPY_LOGE("GetFromDatabase:%s: %s", dbName.c_str(), result->getErrorMessage()->c_str());
            return nullptr;
        }

        auto contents = result->fetch<oatpp::Vector<oatpp::Object<spy::dto::messageContent::MessageContentDto<TContent>>>>();
        if (contents->size() != 1) return nullptr;

        return contents->at(0);
    }

} } }

#endif // spy_ContentWorker
