
#ifndef spy_MessageText
#define spy_MessageText

#include <spy/dto/messageContent/MessageContent.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageText : public oatpp::DTO {
        DTO_INIT(MessageText, MessageContent)

        DTO_FIELD(oatpp::String, text_);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessageText
