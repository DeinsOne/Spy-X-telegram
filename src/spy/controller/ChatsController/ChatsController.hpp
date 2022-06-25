
#ifndef spy_ChatsControllerRest
#define spy_ChatsControllerRest

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/core/utils/ConversionUtils.hpp>

#include <spy/db/ChatsDatabase/ChatsDatabase.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)     //<- Begin Codegen

namespace spy { namespace controller {

    class ChatsController : public oatpp::web::server::api::ApiController {
    public:
        ChatsController(const std::shared_ptr<ObjectMapper>& objectMapper)
          : oatpp::web::server::api::ApiController(objectMapper)
        {}

        static std::shared_ptr<ChatsController> createShared(
            OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
        ) {
            return std::make_shared<ChatsController>(objectMapper);
        }

    public:
        ENDPOINT_INFO(getChatById) {
            info->summary = "Get Chat by chatId";

            info->addResponse<oatpp::Object<dto::Chat>>(Status::CODE_200, "application/text");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_500, "application/text");

            info->pathParams["chatId"].description = "Chat Identifier";
        }
        ENDPOINT("GET", "/chats/{id}", getChatById,
            PATH(oatpp::Int64, chatId, "id")) 
        {
            auto result = chatsDb->getChatById(chatId);

            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
            OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "Chat not found");

            auto chats = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>();
            OATPP_ASSERT_HTTP(chats->size() == 1, Status::CODE_500, "Unknown error");

            return createDtoResponse(Status::CODE_200, chats[0]);
        }


        ENDPOINT_INFO(getChats) {
            info->summary = "Get Chats by chatIds";

            info->addResponse<oatpp::Vector<oatpp::Object<dto::Chat>>>(Status::CODE_200, "application/text");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_500, "application/text");

            info->queryParams["limit"].description = "Maximum number of chats";
            info->queryParams["limit"].required = true;

            info->queryParams["offset"].description = "Chat offset to start parsing from";
            info->queryParams["offset"].required = false;
        }
        ENDPOINT("GET", "/chats", getChats,
            QUERIES(QueryParams, queryParams)) 
        {
            // Check request
            OATPP_ASSERT_HTTP(queryParams.getAll().find("limit") != queryParams.getAll().end(), Status::CODE_400, "Bad parameters. Provide 'limit'");

            // Extract params
            auto offset = 0;
            auto limit = oatpp::utils::conversion::strToInt64(
                (*queryParams.getAll().find("limit")->second.toString()).c_str()
            );
            if (queryParams.getAll().find("offset") != queryParams.getAll().end()) {
                offset = oatpp::utils::conversion::strToInt64(
                    (*queryParams.getAll().find("offset")->second.toString()).c_str()
                );
            }

            // Load from database
            auto result = chatsDb->getChats(limit, offset);

            // Chech db response
            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());

            // Convert result to dto object
            auto chats = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>();

            return createDtoResponse(Status::CODE_200, chats);
        }


        ENDPOINT_INFO(deleteChatById) {
            info->summary = "Delete Chat by chatId";

            info->addResponse<oatpp::Object<dto::Chat>>(Status::CODE_200, "application/text");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_500, "application/text");

            info->pathParams["chatId"].description = "Chat Identifier";
        }
        ENDPOINT("DELETE", "/chats/{id}", deleteChatById,
            PATH(oatpp::Int64, chatId, "id")) 
        {
            // Get chat from database
            auto result = chatsDb->getChatById(chatId);

            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
            OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "Chat not found");

            auto chats = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>();
            OATPP_ASSERT_HTTP(chats->size() == 1, Status::CODE_500, "Unknown error");

            // Delete
            auto deletingResult = chatsDb->deleteChatById(chatId);
            OATPP_ASSERT_HTTP(deletingResult->isSuccess(), Status::CODE_500, deletingResult->getErrorMessage());
            return createDtoResponse(Status::CODE_200, chats[0]);
        }


        ENDPOINT_INFO(updateChat) {
            info->summary = "Update Chat by chatId";

            info->addConsumes<oatpp::Object<dto::Chat>>("application/json");

            info->addResponse<oatpp::Object<dto::Chat>>(Status::CODE_200, "application/text");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_404, "application/json");
            info->addResponse<oatpp::Object<StatusDto>>(Status::CODE_500, "application/text");
        }
        ENDPOINT("PUT", "/chats", updateChat,
            BODY_DTO(oatpp::Object<dto::Chat>, chat))
        {
            // Get chat from database
            auto result = chatsDb->getChatById(chat->id);

            OATPP_ASSERT_HTTP(result->isSuccess(), Status::CODE_500, result->getErrorMessage());
            OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "Chat not found");

            auto chats = result->fetch<oatpp::Vector<oatpp::Object<dto::Chat>>>();
            OATPP_ASSERT_HTTP(chats->size() == 1, Status::CODE_500, "Unknown error");

            // Update chat
            auto updateResult = chatsDb->updateChat(chat);
            OATPP_ASSERT_HTTP(updateResult->isSuccess(), Status::CODE_500, updateResult->getErrorMessage());
            return createDtoResponse(Status::CODE_200, chats[0]);
        }

    private:
        OATPP_COMPONENT(std::shared_ptr<db::ChatsDatabase>, chatsDb);

    };

} } // namespace spy { namespace controller

#include OATPP_CODEGEN_END(ApiController)       //<- End Codegen

#endif // spy_ChatsControllerRest
