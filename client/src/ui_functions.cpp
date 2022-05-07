#include "../include/ui_functions.h"
#include <TGUI/Widgets/Group.hpp>
#include "../include/client.h"
#include "../include/sound_player.h"

namespace war_of_ages {

void place_widgets(std::vector<tgui::Widget::Ptr> &widgets, int width, int height, int space) {
    /// Places widgets in the center of a screen in the order specified by their order in widgets vector.
    ///
    /// Width: width of each Widget in percent of screen width. (default = 40)
    /// Height: height of each Widget in percent of screen height. (default = 10)
    /// Space: a gap between each pair of adjacent Widgets in percent of screen height. (default = 5)
    ///
    /// Normally you don't want to specify width, height or space parameters, because it will lead to
    /// differences between screens.

    int length = static_cast<int>(widgets.size()) * (height + space) - space;
    int cur_y_pos = (100 - length) / 2;
    int x_pos = (100 - width) / 2;
    for (auto &w : widgets) {
        w->setSize(get_layout(width, height));
        w->setPosition(get_layout(x_pos, cur_y_pos));
        cur_y_pos += height + space;
    }
}

void show_screen(tgui::Gui &gui, screen new_screen, screen prev_screen) {
    gui.get(screen_id.at(prev_screen))->setVisible(false);
    gui.get(screen_id.at(new_screen))->setVisible(true);
    current_state.set_cur_screen(new_screen);
    if (current_state.get_cur_game_state() == nullptr && new_screen == screen::GAME_SCREEN) {
        current_state.create_game(client_state::game_mode::SINGLE);
        sound_player::instance().change(sound_player::sound_type::LOBBY, sound_player::sound_type::BATTLE);
    }
    // Settings screen does not contain resume_button
    if (new_screen == screen::SETTINGS) {
        switch (prev_screen) {
            case screen::GAME_SCREEN:
                gui.get<tgui::Group>(screen_id.at(screen::SETTINGS))->get("resume_button")->setVisible(true);
                break;
            case screen::START_SCREEN:
                gui.get<tgui::Group>(screen_id.at(screen::SETTINGS))->get("resume_button")->setVisible(false);
                break;
            default:
                break;
        }
    }
    // return from game to START_SCREEN (be careful with tournaments in the future)
    // TODO: recognize exit from game in another way. I don't sure if this works when music ends
    if (new_screen == screen::START_SCREEN &&
        sound_player::instance().status(sound_player::sound_type::BATTLE) ==
            sf::SoundSource::Status::Playing) {
        current_state.reset_game();
        sound_player::instance().change(sound_player::sound_type::BATTLE, sound_player::sound_type::LOBBY);
    }
    if (new_screen == screen::END_GAME) {
        gui.get(screen_id.at(screen::END_GAME))
            ->cast<tgui::Group>()
            ->get("result_label")
            ->cast<tgui::Label>()
            ->setText(current_state.get_cur_game_state()->get_game_status() == game_status::P1_WON
                          ? "Поздравляем, Вы победили!"
                          : "Вы проиграли, повезет в следующий раз");
        current_state.reset_game();
    }
}

tgui::Layout2d get_layout(int width_percent, int height_percent) {
    return {tgui::String(std::to_string(width_percent) + "%"),
            tgui::String(std::to_string(height_percent) + "%")};
}

void update_fps(std::shared_ptr<tgui::Label> label) {
}

}  // namespace war_of_ages
