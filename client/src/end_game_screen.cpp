#include "../include/end_game_screen.h"
#include "../include/pause_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void end_game_screen_init(tgui::Gui &gui) {
    auto end_game_screen_group = tgui::Group::create();

    auto start_button = tgui::Button::create("В главное меню");
    start_button->setTextSize(30);
    start_button->onPress([&gui]() {
        show_screen(gui, screen::START_SCREEN, screen::END_GAME);
        current_state.set_cur_game_state(nullptr);
    });
    start_button->setPosition("30%", "86%");
    start_button->setSize("40%", "10%");
    end_game_screen_group->add(start_button);

    gui.add(end_game_screen_group, screen_id.at(screen::END_GAME));
    gui.get(screen_id.at(screen::END_GAME))->setVisible(false);
}
}  // namespace war_of_ages