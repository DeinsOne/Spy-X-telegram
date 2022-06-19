
#ifndef spy_FormattedTextToMarkdown
#define spy_FormattedTextToMarkdown

#include <spy/service/functions/IFunction.h>

#include <td/telegram/td_api.hpp>
#include <tdlpp/tdlpp.hpp>

namespace spy { namespace service { namespace functions {

    class FormattedTextToMarkdown : IFunction {
    public:
        FormattedTextToMarkdown(td::td_api::formattedText& formattedText) 
            : formattedText_(formattedText)
        {}

    public:
        virtual void Execute() override {
            markdown = formattedText_.text_;

            try {
            for (auto it = formattedText_.entities_.rbegin(); it != formattedText_.entities_.rend(); ++it) {
                auto& entity = *(*it);
                std::string parsedEntities;

                td::td_api::downcast_call(*entity.type_, tdlpp::overloaded(
                    [&](td::td_api::textEntityTypeBankCardNumber& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeBold& ent) {
                        parsedEntities = ("**" + markdown.substr(entity.offset_, entity.length_) + "**");
                    },
                    [&](td::td_api::textEntityTypeBotCommand& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeCashtag& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeCode& ent) {
                        parsedEntities = ("`" + markdown.substr(entity.offset_, entity.length_) + "`");
                    },
                    [&](td::td_api::textEntityTypeEmailAddress& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeHashtag& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeItalic& ent) {
                        parsedEntities = ("__" + markdown.substr(entity.offset_, entity.length_) + "__");
                    },
                    [&](td::td_api::textEntityTypeMediaTimestamp& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeMention& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypeMentionName& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypePhoneNumber& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypePre& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [&](td::td_api::textEntityTypePreCode& ent) {
                        parsedEntities = ("```" + ent.language_ + markdown.substr(entity.offset_, entity.length_) + "```");
                    },
                    [&](td::td_api::textEntityTypeStrikethrough& ent) {
                        parsedEntities = ("~~" + markdown.substr(entity.offset_, entity.length_) + "~~");
                    },
                    [&](td::td_api::textEntityTypeTextUrl& ent) {
                        parsedEntities = ("[" + markdown.substr(entity.offset_, entity.length_) + "](" + ent.url_ + ")");
                    },
                    [&](td::td_api::textEntityTypeUnderline& ent) {
                        parsedEntities = ("<u>" + markdown.substr(entity.offset_, entity.length_) + "</u>");
                    },
                    [&](td::td_api::textEntityTypeUrl& ent) {
                        parsedEntities = (markdown.substr(entity.offset_, entity.length_));
                    },
                    [](auto&) {}
                ));

                markdown.replace(entity.offset_, entity.length_, parsedEntities);
            }
            }
            catch(...) {
                markdown = formattedText_.text_;
            }
        }

        std::string GetMarkdown() { return markdown; }

    protected:
        td::td_api::formattedText& formattedText_;
        std::string markdown;

    };

} } } // namespace spy { namespace service { namespace functions {

#endif // spy_FormattedTextToMarkdown
