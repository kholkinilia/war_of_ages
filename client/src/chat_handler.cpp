#include "../include/chat_handler.h"
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include "../include/client.h"
#include "../include/screen_handler.h"

namespace war_of_ages::client {

chat_handler::chat_handler() : m_used_colors(COLORS.size(), false) {
    m_used_colors[1] = true;  // Green color for self
    m_players_colors[client::instance().get_handle()] = 1;

    m_chat_group = tgui::Group::create();
    m_chat_group->setPosition("0%", "80%");
    m_chat_group->setSize("25%", "20%");

    m_chat_box = tgui::ChatBox::create();
    m_chat_box->setLinesStartFromTop();
    m_chat_box->setPosition("0%", "0%");
    m_chat_box->setSize("100%", "80%");

    tgui::Texture t;
    t.load("../../client/resources/pictures/chat_background.png");
    m_chat_box->getRenderer()->setTextureBackground(t);
    m_chat_box->setTextStyle(sf::Text::Bold);

    m_edit_box = tgui::EditBox::create();
    m_edit_box->setDefaultText("шитпостите на здоровье...");
    m_edit_box->setPosition("10%", "80%");
    m_edit_box->setSize("90%", "20%");
    m_edit_box->onReturnKeyPress([this](const tgui::String &text) {
        if (text.empty()) {
            return;
        }
        add_message(client::instance().get_handle(), static_cast<std::string>(text));
        message<messages_type> msg;
        msg.header.id = messages_type::CHAT_NEW_MESSAGE;
        msg << m_id;
        msg << static_cast<std::string>(text);
        client::instance().send_message(msg);
        m_edit_box->setText("");
    });

    m_hide_button = tgui::Button::create();
    m_hide_button->setPosition("0%", "80%");
    m_hide_button->setSize("10%", "20%");
    m_hide_button->onClick([this]() {
        if (m_edit_box->isVisible()) {
            m_chat_box->setVisible(false);
            m_edit_box->setVisible(false);
        } else {
            show();
        }
    });

    m_chat_group->add(m_chat_box, "chat_chat_box");
    m_chat_group->add(m_edit_box, "chat_edit_box");
    m_chat_group->add(m_hide_button, "chat_hide_button");
    screen_handler::instance().get_gui().add(m_chat_group, "chat");
}

void chat_handler::add_message(const std::string &handle, const std::string &message) {
    tgui::Color player_color = get_color(handle);
    m_chat_box->addLine(handle + ": " + message, player_color);
}

void chat_handler::remove_player(const std::string &handle) {
    auto color_it = m_players_colors.find(handle);
    if (color_it == m_players_colors.end()) {
        return;
    }
    m_used_colors[color_it->second] = false;
    m_players_colors.erase(color_it);
}

void chat_handler::show() {
    get_chat_box()->setVisible(true);
    get_edit_box()->setVisible(true);
    get_hide_button()->setVisible(true);
}

void chat_handler::hide_totally() {
    get_hide_button()->setVisible(false);
    get_chat_box()->setVisible(false);
    get_edit_box()->setVisible(false);
}

tgui::Color chat_handler::get_color(const std::string &handle) {
    if (m_players_colors.count(handle) == 0) {
        for (std::size_t color_id = COLORS.size() - 1; color_id + 1 != 0; --color_id) {
            if (!m_used_colors[color_id]) {
                if (color_id == 0) {
                    std::cerr << "Too many players in a chat, more than number of reserved colors"
                              << std::endl;
                    // Use black color in this case for any number of players
                } else {
                    m_used_colors[color_id] = true;
                }
                m_players_colors[handle] = color_id;
                break;
            }
        }
    }
    return COLORS[m_players_colors.at(handle)];
}

tgui::Group::Ptr chat_handler::get_chat_group() {
    return m_chat_group;
}

tgui::ChatBox::Ptr chat_handler::get_chat_box() {
    return m_chat_box;
}

tgui::EditBox::Ptr chat_handler::get_edit_box() {
    return m_edit_box;
}

tgui::Button::Ptr chat_handler::get_hide_button() {
    return m_hide_button;
}

void chat_handler::set_chat_id(const std::string &chat_id) {
    m_id = chat_id;
}

void chat_handler::clear() {
    m_chat_box->removeAllLines();
}

}  // namespace war_of_ages::client