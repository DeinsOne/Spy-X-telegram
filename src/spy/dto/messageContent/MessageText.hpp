
#ifndef spy_MessageTextDto
#define spy_MessageTextDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageTextDto : public oatpp::DTO {
        DTO_INIT(MessageTextDto, DTO)

        DTO_FIELD(oatpp::String, text_);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_MessageTextDto
