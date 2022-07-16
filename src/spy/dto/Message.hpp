
#ifndef spy_MessageDto
#define spy_MessageDto

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include <spy/dto/MessageModification.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    class MessageDto : public oatpp::DTO {
        DTO_INIT(MessageDto, DTO)

        DTO_FIELD(oatpp::Int64, id,             "id");
        DTO_FIELD(oatpp::Int64, sender_id,      "sender_id");
        DTO_FIELD(oatpp::Int64, chat_id,        "chat_id");
        DTO_FIELD(oatpp::Int64, album_id,       "media_album_id");
        DTO_FIELD(oatpp::Boolean, can_delete,   "can_be_deleted_for_all");

        DTO_FIELD(oatpp::Vector<oatpp::Object<MessageModificationContentDto>>, modifications);
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessageDto
