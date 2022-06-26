#include <spy/service/functions/SendMessage/SendMessage.hpp>
#include <td/telegram/td_api.hpp>

void spy::service::functions::SendMessage::Execute() {
    auto textParseMarkdown = td::td_api::make_object<td::td_api::textParseModeMarkdown>(1);
    auto parseTextEntities = td::td_api::make_object<td::td_api::parseTextEntities>(message_, std::move(textParseMarkdown));

    td::Client::Request parseRequest { 123, std::move(parseTextEntities) };
    auto parseResponse = td::Client::execute(std::move(parseRequest));

    auto formattedText = td::td_api::make_object<td::td_api::formattedText>();

    if (parseResponse.object->get_id() == td::td_api::formattedText::ID) {
        formattedText = td::td_api::move_object_as<td::td_api::formattedText>(parseResponse.object);
    }
    else {
        std::vector<td::td_api::object_ptr<td::td_api::textEntity>> entities;
        formattedText = td::td_api::make_object<td::td_api::formattedText>(message_, std::move(entities));
    }

    this->handler_->Execute<td::td_api::sendMessage>(
        chatId_, 0, 0,
        td::td_api::make_object<td::td_api::messageSendOptions>(false, true, false, nullptr),
        nullptr,
        td::td_api::make_object<td::td_api::inputMessageText>(
            std::move(formattedText), true, false
        )
    );
}
