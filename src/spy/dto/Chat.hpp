
#ifndef spy_Chat
#define spy_Chat

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    ENUM(ChatType, v_int32,
        VALUE(none, 0),
        VALUE(chatTypeBasicGroup, 1, "BasicGroup"),
        VALUE(chatTypeSupergroup, 2, "Supergroup"),
        VALUE(chatTypePrivate, 3, "Private"),
        VALUE(chatTypeSecret, 4, "Secret")
    )

    class Chat : public oatpp::DTO {
        DTO_INIT(Chat, DTO)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::String, title);
        DTO_FIELD(oatpp::Enum<ChatType>::AsString, type);
        DTO_FIELD(oatpp::Int64, lastMessageId);
        DTO_FIELD(oatpp::String, fetchSince);
        DTO_FIELD(oatpp::String, fetched);
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_Chat
