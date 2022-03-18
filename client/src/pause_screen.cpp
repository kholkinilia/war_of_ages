#include "../include/pause_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void pause_screen_init(tgui::Gui &gui) {
    // TODO: get rid of copy-paste
    auto settings_screen_group = tgui::Group::create();

    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

    tgui::Label::Ptr music_volume_label = tgui::Label::create("Громкость музыки");
    tgui::Label::Ptr sounds_volume_label = tgui::Label::create("Громкость звуков");
    music_volume_label->setTextSize(30);
    sounds_volume_label->setTextSize(30);
    music_volume_label->getRenderer()->setTextColor("white");
    sounds_volume_label->getRenderer()->setTextColor("white");
    music_volume_label->setPosition("34%", "29%");
    sounds_volume_label->setPosition("34%", "39%");

    tgui::Slider::Ptr music_volume_slider = tgui::Slider::create(0, 100);
    tgui::Slider::Ptr sounds_volume_slider = tgui::Slider::create(0, 100);
    music_volume_slider->setRenderer(black_theme.getRenderer("Slider"));
    sounds_volume_slider->setRenderer(black_theme.getRenderer("Slider"));
    music_volume_slider->setPosition("54%", "30%");
    sounds_volume_slider->setPosition("54%", "40%");
    music_volume_slider->setValue(70);
    sounds_volume_slider->setValue(70);

    settings_screen_group->add(music_volume_label);
    settings_screen_group->add(sounds_volume_label);
    settings_screen_group->add(music_volume_slider);
    settings_screen_group->add(sounds_volume_slider);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Продолжить игру");
    return_back_button->setRenderer(black_theme.getRenderer("Button"));
    return_back_button->setTextSize(30);
    return_back_button->onPress([&gui]() { show_screen(gui, screen::GAME_SCREEN, screen::PAUSE); });
    return_back_button->setPosition("30%", "73%");
    return_back_button->setSize("40%", "10%");
    settings_screen_group->add(return_back_button);

    auto start_button = tgui::Button::create("В главное меню");
    start_button->setRenderer(black_theme.getRenderer("Button"));
    start_button->setTextSize(30);
    start_button->onPress([&gui]() {
        show_screen(gui, screen::START_SCREEN, screen::PAUSE);
        current_state.set_cur_game_state(nullptr);
    });
    start_button->setPosition("30%", "86%");
    start_button->setSize("40%", "10%");
    settings_screen_group->add(start_button);

    gui.add(settings_screen_group, screen_id.at(screen::PAUSE));
    gui.get(screen_id.at(screen::PAUSE))->setVisible(false);
}
}  // namespace war_of_ages