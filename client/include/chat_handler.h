#ifndef WAR_OF_AGES_CHAT_HANDLER_H
#define WAR_OF_AGES_CHAT_HANDLER_H

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace war_of_ages::client {
struct chat_handler {
    chat_handler();

    void add_message(const std::string &handle, const std::string &message);
    void remove_player(const std::string &handle);
    void clear();
    void show();
    void hide_totally();

    [[nodiscard]] tgui::Group::Ptr get_chat_group();
    [[nodiscard]] tgui::ChatBox::Ptr get_chat_box();
    [[nodiscard]] tgui::EditBox::Ptr get_edit_box();
    [[nodiscard]] tgui::Button::Ptr get_hide_button();
    [[nodiscard]] tgui::Color get_color(const std::string &handle);

    void set_chat_id(const std::string &chat_id);

private:
    inline static const std::vector<tgui::Color> COLORS{
        {255, 255, 255}, {237, 28, 36},  {255, 127, 39},  {255, 242, 0},  {34, 177, 76},  {0, 162, 232},
        {63, 72, 204},   {163, 73, 164}, {195, 195, 195}, {185, 122, 87}, {255, 174, 201}};

    std::unordered_map<std::string, std::size_t> m_players_colors;
    std::vector<bool> m_used_colors;
    std::string m_id;

    tgui::Group::Ptr m_chat_group;
    tgui::ChatBox::Ptr m_chat_box;
    tgui::EditBox::Ptr m_edit_box;
    tgui::Button::Ptr m_hide_button;
};
}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_CHAT_HANDLER_H
