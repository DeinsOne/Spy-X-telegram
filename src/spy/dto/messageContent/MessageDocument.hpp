
#ifndef spy_MessageDocumentDto
#define spy_MessageDocumentDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageDocumentDto : public oatpp::DTO {
        DTO_INIT(MessageDocumentDto, DTO)

        DTO_FIELD(oatpp::String, caption_);
        DTO_FIELD(oatpp::String, file_id_);
        DTO_FIELD(oatpp::String, mime_type);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_MessageDocumentDto
