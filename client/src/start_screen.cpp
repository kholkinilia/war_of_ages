#include "../include/start_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void start_screen_init(tgui::Gui &gui) {
    auto start_screen_group = tgui::Group::create();

    tgui::Button::Ptr singleplayer_button = tgui::Button::create("Одиночная игра");
    singleplayer_button->setTextSize(30);
    singleplayer_button->onPress([&gui]() {
        show_screen(gui, screen::GAME_SCREEN, screen::START_SCREEN);
    });
    start_screen_group->add(singleplayer_button);

    tgui::Button::Ptr multiplayer_button = tgui::Button::create("Мультиплеер");
    multiplayer_button->setTextSize(30);
    multiplayer_button->onPress([&gui]() {
        show_screen(gui, screen::MULTIPLAYER, screen::START_SCREEN);
    });
    start_screen_group->add(multiplayer_button);

    tgui::Button::Ptr tournament_button = tgui::Button::create("Турниры");
    tournament_button->setTextSize(30);
    tournament_button->onPress([&gui]() {
        show_screen(gui, screen::TOURNAMENT_JOINING, screen::START_SCREEN);
    });
    start_screen_group->add(tournament_button);

    tgui::Button::Ptr statistics_button = tgui::Button::create("Статистика");
    statistics_button->setTextSize(30);
    // TODO: onPress: switch to the statistics screen when it is implemented
    start_screen_group->add(statistics_button);

    tgui::Button::Ptr exit_button = tgui::Button::create("Выйти");
    exit_button->setTextSize(30);
    exit_button->onPress(exit, 0);  // FIXME: Implement closing app in the other way
    start_screen_group->add(exit_button);

    std::vector<tgui::Widget::Ptr> widgets{singleplayer_button, multiplayer_button, tournament_button,
                                           statistics_button, exit_button};
    place_widgets(widgets);

    tgui::Theme settings_icon_theme("../client/resources/tgui_themes/settings_icon.txt");
    tgui::Button::Ptr settings_screen_button = tgui::Button::create();
    settings_screen_button->setRenderer(settings_icon_theme.getRenderer("Button"));
    settings_screen_button->setSize("4.5%", "8%");
    settings_screen_button->setPosition("90%", "5%");
    settings_screen_button->onPress(
        [&gui]() { show_screen(gui, screen::SETTINGS, screen::START_SCREEN); });
    start_screen_group->add(settings_screen_button);

    gui.add(start_screen_group, screen_id.at(screen::START_SCREEN));
    gui.get(screen_id.at(screen::START_SCREEN))->setVisible(false);
}
}  // namespace war_of_ages