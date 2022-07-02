
#ifndef spy_MarkdownToFormattedTextDunction
#define spy_MarkdownToFormattedTextDunction

#include <spy/service/functions/IFunction.h>

#include <td/telegram/td_api.hpp>
#include <tdlpp/tdlpp.hpp>

namespace spy { namespace service { namespace functions {

    class MarkdownToFormattedText : IFunction {
    public:
        MarkdownToFormattedText(const std::string& markdown) 
            : markdown_(markdown)
        {}

    public:
        virtual void Execute() override {
        try {
            auto textParseMarkdown = td::td_api::make_object<td::td_api::textParseModeMarkdown>(1);
            auto parseTextEntities = td::td_api::make_object<td::td_api::parseTextEntities>(markdown_, std::move(textParseMarkdown));

            td::Client::Request parseRequest { 123, std::move(parseTextEntities) };
            auto parseResponse = td::Client::execute(std::move(parseRequest));

            auto formattedText = td::td_api::make_object<td::td_api::formattedText>();

            if (parseResponse.object->get_id() == td::td_api::formattedText::ID) {
                formattedText = td::td_api::move_object_as<td::td_api::formattedText>(parseResponse.object);
            }
            else {
                std::vector<td::td_api::object_ptr<td::td_api::textEntity>> entities;
                formattedText = td::td_api::make_object<td::td_api::formattedText>(markdown_, std::move(entities));
            }

            formattedText_ = std::shared_ptr<td::td_api::formattedText>(formattedText.release());
        }
        catch(...) { }
        }

        std::shared_ptr<td::td_api::formattedText> GetFormattedText() { return formattedText_; }

    protected:
        std::shared_ptr<td::td_api::formattedText> formattedText_;
        std::string markdown_;

    };

} } } // namespace spy { namespace service { namespace functions {

#endif // spy_MarkdownToFormattedTextDunction
