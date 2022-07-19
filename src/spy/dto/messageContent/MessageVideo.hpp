
#ifndef spy_MessageVideoDto
#define spy_MessageVideoDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageVideoDto : public oatpp::DTO {
        DTO_INIT(MessageVideoDto, DTO)

        DTO_FIELD(oatpp::String, caption_);
        DTO_FIELD(oatpp::Boolean, is_secret_);
        DTO_FIELD(oatpp::String, file_id_);
        DTO_FIELD(oatpp::Int32, duration);
        DTO_FIELD(oatpp::Int32, width);
        DTO_FIELD(oatpp::Int32, height);
        DTO_FIELD(oatpp::String, mime_type);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_MessageVideoDto
