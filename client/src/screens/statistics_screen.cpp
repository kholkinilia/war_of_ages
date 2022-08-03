#include "client.h"
#include "messages_type.h"
#include "screen_handler.h"

// TGUI widgets
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>

namespace war_of_ages::client {

void screen_handler::statistics_screen_init() {
    auto statistics_screen_group = tgui::Group::create();

    auto handle_rating_label = tgui::Label::create("Здесь будут хендл и рейтинг");
    handle_rating_label->setTextSize(30);
    handle_rating_label->setPosition("0%", "0%");
    handle_rating_label->setSize("100%", "10%");
    handle_rating_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    statistics_screen_group->add(handle_rating_label, "handle_rating_label");

    auto panel = tgui::ScrollablePanel::create();
    tgui::Panel::Ptr background;
    tgui::Label::Ptr my_handle, my_rating, opponent_handle, opponent_rating;
    for (int i = 0; i < 16; i++) {
        background = tgui::Panel::create({"60%", "10%"});
        my_handle = tgui::Label::create("my_handle");
        my_rating = tgui::Label::create("my_rating");
        opponent_handle = tgui::Label::create("opponent_handle");
        opponent_rating = tgui::Label::create("opponent_rating");

        background->getRenderer()->setBackgroundColor(i % 2 == 1 ? tgui::Color::Yellow : tgui::Color::Black);
        background->getRenderer()->setBorderColor(tgui::Color::Black);
        background->getRenderer()->setBorders(2);

        my_handle->setSize({"15%", "100%"});
        my_rating->setSize({"15%", "100%"});
        opponent_handle->setSize({"15%", "100%"});
        opponent_rating->setSize({"15%", "100%"});

        background->setPosition(0.2 * 1920, 0.1 * 1080 * i + 108);
        my_handle->setPosition({"0%", "0%"});
        my_rating->setPosition({"25%", "0%"});
        opponent_handle->setPosition({"50%", "0%"});
        opponent_rating->setPosition({"75%", "0%"});

        my_handle->setTextSize(35);
        my_rating->setTextSize(35);
        opponent_handle->setTextSize(35);
        opponent_rating->setTextSize(35);

        my_handle->getRenderer()->setTextColor(tgui::Color::Black);
        my_rating->getRenderer()->setTextColor(tgui::Color::Black);
        opponent_handle->getRenderer()->setTextColor(tgui::Color::Black);
        opponent_rating->getRenderer()->setTextColor(tgui::Color::Black);

        auto border1 = tgui::Panel::create({"0.2%", "100%"}), border2 = tgui::Panel::create({"0.2%", "100%"}),
             border3 = tgui::Panel::create({"0.2%", "100%"});
        border1->getRenderer()->setBackgroundColor(tgui::Color::Black);
        border1->setPosition({"24.7%", "0%"});
        border2->getRenderer()->setBackgroundColor(tgui::Color::Black);
        border2->setPosition({"49.7%", "0%"});
        border3->getRenderer()->setBackgroundColor(tgui::Color::Black);
        border3->setPosition({"74.7%", "0%"});

        background->add(border1);
        background->add(border2);
        background->add(border3);
        background->add(my_handle, "my_handle");
        background->add(my_rating, "my_rating");
        background->add(opponent_handle, "opponent_handle");
        background->add(opponent_rating, "opponent_rating");
        panel->add(background, "background_" + std::to_string(i));
    }
    panel->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    statistics_screen_group->add(panel, "stats_panel");

    tgui::Theme black_theme("../../client/resources/tgui_themes/Black.txt");
    auto return_button = tgui::Button::create("Назад");
    return_button->setRenderer(black_theme.getRenderer("Button"));
    return_button->setTextSize(30);
    return_button->onPress(
        [&]() { screen_handler::instance().change_screen(screen_handler::screen_type::PREVIOUS_MENU); });
    return_button->setPosition("30%", "86%");
    return_button->setSize("40%", "10%");
    statistics_screen_group->add(return_button, "return_button");
    m_gui.add(statistics_screen_group, screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS));
    m_gui.get(screen_handler::screen_id.at(screen_handler::screen_type::STATISTICS))->setVisible(false);
}
}  // namespace war_of_ages::client
