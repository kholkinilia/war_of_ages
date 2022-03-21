#include "../include/settings_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void settings_screen_init(sf::View &v, tgui::Gui &gui) {
    // TODO: try make this shit more readable and well-formed
    auto settings_screen_group = tgui::Group::create();

    tgui::Theme black_theme("../client/resources/tgui_themes/Black.txt");

    tgui::Label::Ptr battle_music_volume_label = tgui::Label::create("Громкость музыки в бою");
    tgui::Label::Ptr battle_sounds_volume_label = tgui::Label::create("Громкость звуков в бою");
    tgui::Label::Ptr lobby_music_volume_label = tgui::Label::create("Громкость музыки в лобби");
    battle_music_volume_label->setTextSize(25);
    battle_sounds_volume_label->setTextSize(25);
    lobby_music_volume_label->setTextSize(25);
    battle_music_volume_label->getRenderer()->setTextColor("white");
    battle_sounds_volume_label->getRenderer()->setTextColor("white");
    lobby_music_volume_label->getRenderer()->setTextColor("white");
    battle_music_volume_label->setPosition("34%", "29%");
    battle_sounds_volume_label->setPosition("34%", "39%");
    lobby_music_volume_label->setPosition("34%", "49%");

    tgui::Slider::Ptr battle_music_volume_slider = tgui::Slider::create(0, 100);
    tgui::Slider::Ptr battle_sounds_volume_slider = tgui::Slider::create(0, 100);
    tgui::Slider::Ptr lobby_music_volume_slider = tgui::Slider::create(0, 100);
    battle_music_volume_slider->setRenderer(black_theme.getRenderer("Slider"));
    battle_sounds_volume_slider->setRenderer(black_theme.getRenderer("Slider"));
    lobby_music_volume_slider->setRenderer(black_theme.getRenderer("Slider"));
    battle_music_volume_slider->setPosition("54%", "30%");
    battle_sounds_volume_slider->setPosition("54%", "40%");
    lobby_music_volume_slider->setPosition("54%", "50%");

    battle_music_volume_slider->onValueChange(
        [&battle_sounds_volume_slider](float new_value) { current_state.battle_music.setVolume(new_value); });
    lobby_music_volume_slider->onValueChange(
        [&lobby_music_volume_slider](float new_value) { current_state.lobby_music.setVolume(new_value); });
    battle_music_volume_slider->setValue(current_state.battle_music.getVolume());
    battle_sounds_volume_slider->setValue(50);
    lobby_music_volume_slider->setValue(current_state.lobby_music.getVolume());

    settings_screen_group->add(battle_music_volume_label);
    settings_screen_group->add(battle_sounds_volume_label);
    settings_screen_group->add(lobby_music_volume_label, "lobby_music_volume_label");
    settings_screen_group->add(battle_music_volume_slider);
    settings_screen_group->add(battle_sounds_volume_slider);
    settings_screen_group->add(lobby_music_volume_slider, "lobby_music_volume_slider");

    tgui::Button::Ptr resume_button = tgui::Button::create("Продолжить игру");
    resume_button->setRenderer(black_theme.getRenderer("Button"));
    resume_button->setTextSize(30);
    resume_button->onPress([&gui, &v]() {
        current_state.get_cur_game_state()->set_time_after_pause(1.f * clock() / CLOCKS_PER_SEC);
        v.setCenter(current_state.get_view_center());
        show_screen(gui, screen::GAME_SCREEN, screen::SETTINGS);
    });
    resume_button->setPosition("30%", "73%");
    resume_button->setSize("40%", "10%");
    settings_screen_group->add(resume_button, "resume_button");

    auto start_button = tgui::Button::create("В главное меню");
    start_button->setRenderer(black_theme.getRenderer("Button"));
    start_button->setTextSize(30);
    start_button->onPress([&gui]() { show_screen(gui, screen::START_SCREEN, screen::SETTINGS); });
    start_button->setPosition("30%", "86%");
    start_button->setSize("40%", "10%");
    settings_screen_group->add(start_button);

    gui.add(settings_screen_group, screen_id.at(screen::SETTINGS));
    gui.get(screen_id.at(screen::SETTINGS))->setVisible(false);
}
}  // namespace war_of_ages
