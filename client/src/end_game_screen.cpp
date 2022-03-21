#include "../include/end_game_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void end_game_screen_init(tgui::Gui &gui) {
    auto end_game_screen_group = tgui::Group::create();

    auto result = tgui::Label::create();
    result->getRenderer()->setTextSize(40);
    result->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    result->getRenderer()->setTextColor(tgui::Color::White);
    end_game_screen_group->add(result, "result_label");

    auto start_button = tgui::Button::create("В главное меню");
    start_button->setTextSize(30);
    start_button->onPress([&gui]() {
        show_screen(gui, screen::START_SCREEN, screen::END_GAME);
        // FIXME: WTF why is it here? We definitely need a .reset() member function
        war_of_ages::current_state.battle_music.stop();
    });
    end_game_screen_group->add(start_button);

    std::vector<tgui::Widget::Ptr> widgets = {result, start_button};
    place_widgets(widgets);

    gui.add(end_game_screen_group, screen_id.at(screen::END_GAME));
    gui.get(screen_id.at(screen::END_GAME))->setVisible(false);
}
}  // namespace war_of_ages