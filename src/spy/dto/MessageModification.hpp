
#ifndef spy_MessageModification
#define spy_MessageModification

#include <spy/dto/messageContent/MessageContent.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto {

    class MessageModificationDto : public oatpp::DTO {
        DTO_INIT(MessageModificationDto, DTO)

        DTO_FIELD(oatpp::Int64, id);
        DTO_FIELD(oatpp::Int64, chat_id);
        DTO_FIELD(oatpp::Int32, version_);
        DTO_FIELD(oatpp::String, message_content);
        DTO_FIELD(oatpp::String, date_);

        DTO_FIELD(oatpp::Object<messageContent::MessageContent>, content);
    };


} } // namespace spy { namespace dto {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessageModification
