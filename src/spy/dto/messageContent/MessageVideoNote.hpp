
#ifndef spy_MessageVideoNoteDto
#define spy_MessageVideoNoteDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessageVideoNoteDto : public oatpp::DTO {
        DTO_INIT(MessageVideoNoteDto, DTO)

        DTO_FIELD(oatpp::Boolean, is_secret_);
        DTO_FIELD(oatpp::Boolean, is_viewed);
        DTO_FIELD(oatpp::String, file_id_);
        DTO_FIELD(oatpp::Int32, duration);
        DTO_FIELD(oatpp::Int32, length);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif // spy_MessageVideoNoteDto
