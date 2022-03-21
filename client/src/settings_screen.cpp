#include "../include/settings_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void settings_screen_init(sf::View &v, tgui::Gui &gui) {
    // TODO: try make this shit more readable and well-formed
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
    music_volume_slider->setValue(50);
    sounds_volume_slider->setValue(50);

    music_volume_slider->onValueChange(
        [&music_volume_slider](float new_value) { current_state.battle_music.setVolume(new_value); });

    settings_screen_group->add(music_volume_label);
    settings_screen_group->add(sounds_volume_label);
    settings_screen_group->add(music_volume_slider);
    settings_screen_group->add(sounds_volume_slider);

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
    start_button->onPress([&gui]() {
        show_screen(gui, screen::START_SCREEN, screen::SETTINGS);
        if (current_state.battle_music.getStatus() == sf::SoundSource::Status::Playing) {
            // Game is running
            current_state.set_cur_game_state(nullptr);
            // FIXME: WTF why is it here? We definitely need a .reset() member function
            war_of_ages::current_state.battle_music.stop();
        }
    });
    start_button->setPosition("30%", "86%");
    start_button->setSize("40%", "10%");
    settings_screen_group->add(start_button);

    gui.add(settings_screen_group, screen_id.at(screen::SETTINGS));
    gui.get(screen_id.at(screen::SETTINGS))->setVisible(false);
}
}  // namespace war_of_ages
