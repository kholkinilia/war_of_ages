#include "../include/tournament_screens.h"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <mutex>


#include "../include/ui_functions.h"

namespace war_of_ages {

void tournament_join_screen_init(tgui::Gui &gui) {
    auto tournament_join_screen_group = tgui::Group::create();

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_CREATION_SCREEN, TOURNAMENT_JOIN_SCREEN); });
    tournament_join_screen_group->add(create_tournament_button);

    tgui::EditBox::Ptr tournament_key_box = tgui::EditBox::create();
    tournament_key_box->setInputValidator("[a-zA-Z0-9]{0,20}");
    tournament_key_box->setTextSize(30);
    tournament_key_box->setDefaultText("введите код доступа к турнуру");
    tournament_join_screen_group->add(tournament_key_box);

    tgui::Button::Ptr join_tournament_button = tgui::Button::create("Присоединиться");
    join_tournament_button->setTextSize(30);
    tournament_join_screen_group->add(join_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(exit, 0);  // TODO: set valid screen view function
    tournament_join_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{create_tournament_button, tournament_key_box,
                                           join_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_join_screen_group, TOURNAMENT_JOIN_SCREEN);
    gui.get(TOURNAMENT_JOIN_SCREEN)->setVisible(false);
}

void tournament_creation_screen_init(tgui::Gui &gui) {
    auto tournament_creation_screen_group = tgui::Group::create();

    tgui::EditBox::Ptr tournament_name_box = tgui::EditBox::create();
    tournament_name_box->setInputValidator("[a-zA-Z0-9а-яА-Я ]{0,35}");
    tournament_name_box->setTextSize(30);
    tournament_name_box->setDefaultText("Введите название турнира");
    tournament_creation_screen_group->add(tournament_name_box);

    tgui::EditBox::Ptr participants_number_box = tgui::EditBox::create();
    participants_number_box->setInputValidator(tgui::EditBox::Validator::UInt);
    participants_number_box->setTextSize(30);
    participants_number_box->setDefaultText("Введите кол-во человек в турнире");
    tournament_creation_screen_group->add(participants_number_box);

    tgui::Button::Ptr create_tournament_button = tgui::Button::create("Создать турнир");
    create_tournament_button->setTextSize(30);
    create_tournament_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_SCREEN, TOURNAMENT_CREATION_SCREEN); });
    tournament_creation_screen_group->add(create_tournament_button);

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress(
        [&] { show_screen(gui, TOURNAMENT_JOIN_SCREEN, TOURNAMENT_CREATION_SCREEN); });
    tournament_creation_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{tournament_name_box, participants_number_box,
                                           create_tournament_button, return_back_button};

    place_widgets(widgets);

    gui.add(tournament_creation_screen_group, TOURNAMENT_CREATION_SCREEN);
    gui.get(TOURNAMENT_CREATION_SCREEN)->setVisible(false);
}

void tournament_screen_init(tgui::Gui &gui) {
    // TODO: implement to the end

    auto tournament_screen_group = tgui::Group::create();

    tgui::Button::Ptr return_back_button = tgui::Button::create("Назад");
    return_back_button->setTextSize(30);
    return_back_button->onPress([&] { show_screen(gui, TOURNAMENT_JOIN_SCREEN, TOURNAMENT_SCREEN); });
    tournament_screen_group->add(return_back_button);

    std::vector<tgui::Widget::Ptr> widgets{return_back_button};

    place_widgets(widgets);

    gui.add(tournament_screen_group, TOURNAMENT_SCREEN);
    gui.get(TOURNAMENT_SCREEN)->setVisible(false);
}

void tournament::update_places_lock_held() {
    std::vector<int> cnt(part_number * WIN_POINTS + 1);
    std::vector<int> final_place(part_number * WIN_POINTS + 1);
    for (int i = 0; i < part_number; i++) {
        cnt[sum[i]]++;
    }
    int cur_place = 1;
    for (int i = part_number * WIN_POINTS; i >= 0; i--) {
        final_place[i] = cur_place;
        cur_place += cnt[i];
    }

    for (int i = 0; i < part_number; i++) {
        place[i] = final_place[sum[i]];
    }
}

void tournament::add_participant(const tgui::String &handle) {
    std::unique_lock lock(m);
    if (id.count(handle) != 0) {
        assert(false);
        return;
    }
    participants.push_back(handle);
    id[handle] = part_number;
    for (auto &participant_results : match_results) {
        participant_results.emplace_back(result::NONE);
    }
    match_results.emplace_back(part_number + 1);
    sum.emplace_back(0);
    place.emplace_back(0);
    part_number++;
    update_places_lock_held();
}

void tournament::add_result(const tgui::String &winner, const tgui::String &loser) {
    std::unique_lock lock(m);
    match_results[id[winner]][id[loser]] = result::VICTORY;
    match_results[id[loser]][id[winner]] = result::DEFEAT;
    sum[id[winner]] += WIN_POINTS;
    update_places_lock_held();
}

void tournament::remove_participant(const tgui::String &handle) {
    std::unique_lock lock(m);
    int remove_id = id[handle];
    for (int i = 0; i < part_number; i++) {
        if (match_results[remove_id][i] == result::DEFEAT) {
            sum[i] -= WIN_POINTS;
        }
    }
    for (auto &part_results : match_results) {
        part_results.erase(part_results.begin() + remove_id);
    }
    for (auto &cur_handle : participants) {
        if (id[cur_handle] > remove_id) {
            id[cur_handle]--;
        }
    }
    id.erase(participants[remove_id]);
    match_results.erase(match_results.begin() + remove_id);
    sum.erase(sum.begin() + remove_id);
    place.erase(place.begin() + remove_id);
    participants.erase(participants.begin() + remove_id);
    part_number--;
    update_places_lock_held();
}

void tournament::update_grid(const tgui::Grid::Ptr &grid) {
    // TODO: think of improving performance (should be easy)

    std::unique_lock lock(m);
    // table parameters
    static const int HANDLE_WIDTH = 300;
    static const int SQUARE_SIZE = 50;
    static const int TEXT_SIZE = 25;
    static const int THICK_BORDER_WIDTH = 3;
    static const int REGULAR_BORDER_WIDTH = 3;
    static const tgui::Color BORDER_COLOR = tgui::Color::Black;
    static const tgui::Color REGULAR_BACKGROUND_COLOR = tgui::Color(100, 100, 100);
    static const tgui::Color BLANK_CELL_BACKGROUND_COLOR = tgui::Color(50, 50, 50);
    static const tgui::Color TEXT_COLOR = tgui::Color::White;
    static const std::map<result, std::string> result_text{
        {result::NONE, "."}, {result::VICTORY, "V"}, {result::DEFEAT, "D"}};
    static const std::map<result, tgui::Color> result_color{{result::NONE, REGULAR_BACKGROUND_COLOR},
                                                            {result::VICTORY, tgui::Color(0, 100, 0)},
                                                            {result::DEFEAT, tgui::Color(100, 0, 0)}};
    // end table parameters

    grid->removeAllWidgets();

    static auto format_label = [=](const tgui::Label::Ptr &label, int width, int border_width,
                                   tgui::Color background_color) {
        label->setSize(width, SQUARE_SIZE);
        label->setTextSize(TEXT_SIZE);
        label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
        label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
        label->getRenderer()->setBorders(border_width);
        label->getRenderer()->setBorderColor(BORDER_COLOR);
        label->getRenderer()->setBackgroundColor(background_color);
        label->getRenderer()->setTextColor(TEXT_COLOR);
    };

    tgui::Label::Ptr handle_label = tgui::Label::create("Хэндл");
    format_label(handle_label, HANDLE_WIDTH, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
    grid->addWidget(handle_label, 0, 0);

    for (int i = 0; i < part_number; i++) {
        tgui::Label::Ptr handle = tgui::Label::create(participants[i]);
        format_label(handle, HANDLE_WIDTH, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        grid->addWidget(handle, i + 1, 0);
    }

    for (int i = 0; i < part_number; i++) {
        tgui::Label::Ptr number = tgui::Label::create(std::to_string(i + 1));
        format_label(number, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        grid->addWidget(number, i + 1, 1);
    }

    for (int i = 0; i < part_number; i++) {  // TODO: make it not be a copy-paste
        tgui::Label::Ptr number = tgui::Label::create(std::to_string(i + 1));
        format_label(number, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        grid->addWidget(number, 0, i + 2);
    }

    for (int i = 0; i < part_number + 1; i++) {
        tgui::Label::Ptr blank = tgui::Label::create();
        format_label(blank, SQUARE_SIZE, REGULAR_BORDER_WIDTH, BLANK_CELL_BACKGROUND_COLOR);
        grid->addWidget(blank, i, i + 1);
    }

    for (int i = 0; i < part_number; i++) {
        for (int j = 0; j < part_number; j++) {
            if (i == j)
                continue;
            tgui::Label::Ptr cur_result = tgui::Label::create(result_text.at(match_results[i][j]));
            format_label(cur_result, SQUARE_SIZE, REGULAR_BORDER_WIDTH, result_color.at(match_results[i][j]));
            grid->addWidget(cur_result, i + 1, j + 2);
        }
    }

    tgui::Label::Ptr sum_label = tgui::Label::create("С");
    tgui::Label::Ptr sum_tool_tip = tgui::Label::create("Суммарное кол-во очков");
    sum_tool_tip->getRenderer()->setBackgroundColor(REGULAR_BACKGROUND_COLOR);
    sum_tool_tip->getRenderer()->setTextColor(TEXT_COLOR);
    sum_label->setToolTip(sum_tool_tip);
    format_label(sum_label, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
    grid->addWidget(sum_label, 0, part_number + 2);

    tgui::Label::Ptr place_label = tgui::Label::create("М");
    tgui::Label::Ptr place_tool_tip = tgui::Label::create("Текущее место");
    place_tool_tip->getRenderer()->setBackgroundColor(REGULAR_BACKGROUND_COLOR);
    place_tool_tip->getRenderer()->setTextColor(TEXT_COLOR);
    place_label->setToolTip(place_tool_tip);
    format_label(place_label, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
    grid->addWidget(place_label, 0, part_number + 3);

    for (int i = 0; i < part_number; i++) {
        tgui::Label::Ptr part_sum = tgui::Label::create(std::to_string(sum[i]));
        format_label(part_sum, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        grid->addWidget(part_sum, i + 1, part_number + 2);

        tgui::Label::Ptr part_place = tgui::Label::create(std::to_string(place[i]));
        format_label(part_place, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        grid->addWidget(part_place, i + 1, part_number + 3);
    }
}

}  // namespace war_of_ages
