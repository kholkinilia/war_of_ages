#include "../include/settings_screen.h"
#include "../include/screen_defines.h"
#include "../include/ui_functions.h"

namespace war_of_ages {
void settings_screen_init(tgui::Gui &gui) {
    // TL;DR TODO: try make this shit more readable and well-formed
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

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setRenderer(black_theme.getRenderer("Button"));
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&gui]() { show_screen(gui, screen::START_SCREEN, screen::SETTINGS); });
    return_back_button->setPosition("30%", "70%");
    return_back_button->setSize("40%", "10%");
    settings_screen_group->add(return_back_button);

    gui.add(settings_screen_group, screen_id.at(screen::SETTINGS));
    gui.get(screen_id.at(screen::SETTINGS))->setVisible(false);
}
}  // namespace war_of_ages