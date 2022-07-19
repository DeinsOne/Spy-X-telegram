
#ifndef spy_MessagesControllerRest
#define spy_MessagesControllerRest

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <spy/db/MessagesDatabase/MessagesDatabase.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

namespace spy { namespace controller {

    class MessagesController : public oatpp::web::server::api::ApiController {
    public:
        MessagesController(const std::shared_ptr<ObjectMapper>& objectMapper)
          : oatpp::web::server::api::ApiController(objectMapper)
        {}

        static std::shared_ptr<MessagesController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
        ) {
            return std::make_shared<MessagesController>(objectMapper);
        }

    public:
        ENDPOINT("GET", "/messages/message", GetMessageById,
            QUERY(oatpp::Int64, chat_id),
            QUERY(oatpp::Int64, message_id)
        ) {
            auto message = messagesDb->GetMessageById(chat_id, message_id);
            OATPP_ASSERT_HTTP(message != nullptr, Status::CODE_404, "Message was not found");
            return createDtoResponse(Status::CODE_200, message);
        }

        ENDPOINT("GET", "/messages/messages", GetChatMessages,
            QUERY(oatpp::Int64, chat_id),
            QUERY(oatpp::Int64, first_id),
            QUERY(oatpp::Int32, limit)
        ) {
            auto messages = messagesDb->GetChatMessages(chat_id, first_id, limit);
            OATPP_ASSERT_HTTP(messages != nullptr, Status::CODE_404, "Message was not found");
            return createDtoResponse(Status::CODE_200, messages);
        }

        ENDPOINT("GET", "/messages/last", GetChatLastMessage,
            QUERY(oatpp::Int64, chat_id)
        ) {
            auto message = messagesDb->GetChatLastMessage(chat_id);
            OATPP_ASSERT_HTTP(message != nullptr, Status::CODE_404, "Message was not found");
            return createDtoResponse(Status::CODE_200, message);
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::MessagesDatabase>, messagesDb);

    };

} } // namespace spy { namespace controller

#include OATPP_CODEGEN_END(ApiController)

#endif // spy_MessagesControllerRest
