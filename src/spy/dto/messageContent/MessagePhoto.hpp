
#ifndef spy_MessagePhotoDto
#define spy_MessagePhotoDto

#include OATPP_CODEGEN_BEGIN(DTO)

namespace spy { namespace dto { namespace messageContent {

    class MessagePhotoDto : public oatpp::DTO {
        DTO_INIT(MessagePhotoDto, DTO)

        DTO_FIELD(oatpp::String, caption_);
        DTO_FIELD(oatpp::Boolean, is_secret_);
        DTO_FIELD(oatpp::String, file_id_);
        DTO_FIELD(oatpp::Int32, width);
        DTO_FIELD(oatpp::Int32, height);
    };


} } } // namespace spy { namespace dto { namespace messageContent {

#include OATPP_CODEGEN_END(DTO)

#endif //spy_MessagePhotoDto
