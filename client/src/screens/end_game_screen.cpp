#include "../../include/screen_handler.h"
#include "../../include/sound_player.h"
#include "../../include/bot_actions_supplier.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void screen_handler::end_game_screen_init() {
    auto end_game_screen_group = tgui::Group::create();

    auto result = tgui::Label::create();
    result->getRenderer()->setTextSize(35);
    result->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    result->getRenderer()->setTextColor(tgui::Color::White);
    end_game_screen_group->add(result, "result_label");

    auto start_button = tgui::Button::create("Ок...");
    start_button->setTextSize(30);
    start_button->onPress([&]() {
        // bot_actions_supplier::write_to_file();
        screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU);
        sound_player::instance().change(sound_player::sound_type::BATTLE, sound_player::sound_type::LOBBY);
    });
    end_game_screen_group->add(start_button);

    std::vector<tgui::Widget::Ptr> widgets = {result, start_button};
    place_widgets(widgets);

    m_gui.add(end_game_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::END_GAME));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::END_GAME))->setVisible(false);
}
}  // namespace war_of_ages::client