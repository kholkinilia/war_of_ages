#include "../include/tournament_handler.h"
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages::client {

void tournament_handler::update_grid() {
    // TODO: think of improving performance (should be easy)
    std::unique_lock lock(m_mutex);
    if (m_is_grid_updated) {
        return;
    }
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
    static const std::map<game_result, std::string> result_text{
        {game_result::NONE, "."}, {game_result::VICTORY, "V"}, {game_result::DEFEAT, "D"}};
    static const std::map<game_result, tgui::Color> result_color{
        {game_result::NONE, REGULAR_BACKGROUND_COLOR},
        {game_result::VICTORY, tgui::Color(0, 100, 0)},
        {game_result::DEFEAT, tgui::Color(100, 0, 0)}};
    // end table parameters

    m_grid->removeAllWidgets();

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
    m_grid->addWidget(handle_label, 0, 0);

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        tgui::Label::Ptr handle = tgui::Label::create(m_participants[i]);
        format_label(handle, HANDLE_WIDTH, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        m_grid->addWidget(handle, i + 1, 0);
    }

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        tgui::Label::Ptr number = tgui::Label::create(std::to_string(i + 1));
        format_label(number, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        m_grid->addWidget(number, i + 1, 1);
    }

    for (std::size_t i = 0; i < m_participants.size(); i++) {  // TODO: make it not be a copy-paste
        tgui::Label::Ptr number = tgui::Label::create(std::to_string(i + 1));
        format_label(number, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        m_grid->addWidget(number, 0, i + 2);
    }

    for (std::size_t i = 0; i <= m_participants.size(); i++) {
        tgui::Label::Ptr blank = tgui::Label::create();
        format_label(blank, SQUARE_SIZE, REGULAR_BORDER_WIDTH, BLANK_CELL_BACKGROUND_COLOR);
        m_grid->addWidget(blank, i, i + 1);
    }

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        for (std::size_t j = 0; j < m_participants.size(); j++) {
            if (i == j)
                continue;
            tgui::Label::Ptr cur_result = tgui::Label::create(result_text.at(m_match_results[i][j]));
            format_label(cur_result, SQUARE_SIZE, REGULAR_BORDER_WIDTH,
                         result_color.at(m_match_results[i][j]));
            m_grid->addWidget(cur_result, i + 1, j + 2);
        }
    }

    tgui::Label::Ptr sum_label = tgui::Label::create("С");
    tgui::Label::Ptr sum_tool_tip = tgui::Label::create("Суммарное кол-во очков");
    sum_tool_tip->getRenderer()->setBackgroundColor(REGULAR_BACKGROUND_COLOR);
    sum_tool_tip->getRenderer()->setTextColor(TEXT_COLOR);
    sum_label->setToolTip(sum_tool_tip);
    format_label(sum_label, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
    m_grid->addWidget(sum_label, 0, m_participants.size() + 2);

    tgui::Label::Ptr place_label = tgui::Label::create("М");
    tgui::Label::Ptr place_tool_tip = tgui::Label::create("Текущее место");
    place_tool_tip->getRenderer()->setBackgroundColor(REGULAR_BACKGROUND_COLOR);
    place_tool_tip->getRenderer()->setTextColor(TEXT_COLOR);
    place_label->setToolTip(place_tool_tip);
    format_label(place_label, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
    m_grid->addWidget(place_label, 0, m_participants.size() + 3);

    for (std::size_t i = 0; i < m_participants.size(); i++) {
        tgui::Label::Ptr part_sum = tgui::Label::create(std::to_string(m_sum[i]));
        format_label(part_sum, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        m_grid->addWidget(part_sum, i + 1, m_participants.size() + 2);

        tgui::Label::Ptr part_place = tgui::Label::create(std::to_string(m_place[i]));
        format_label(part_place, SQUARE_SIZE, THICK_BORDER_WIDTH, REGULAR_BACKGROUND_COLOR);
        m_grid->addWidget(part_place, i + 1, m_participants.size() + 3);
    }

    m_is_grid_updated = true;
}

void tournament_handler::post_add_participant(const std::string &) {
    m_is_grid_updated = false;
}

void tournament_handler::post_add_result(const std::string &, const std::string &) {
    m_is_grid_updated = false;
}

void tournament_handler::post_remove_participant(const std::string &) {
    m_is_grid_updated = false;
}

void tournament_handler::set_grid(tgui::Grid::Ptr &grid) {
    m_grid = grid;
}

void tournament_handler::set_name_label(tgui::Label::Ptr &name_label) {
    m_name_label = name_label;
}

void tournament_handler::set_key_label(tgui::Label::Ptr &key_label) {
    m_key_label = key_label;
}

void tournament_handler::set_tournament(const tournament_snapshot &snapshot) {
    std::unique_lock lock(m_mutex);
    m_name = snapshot.name;
    m_name_label->setText(m_name);
    m_key = snapshot.key;
    m_key_label->setText("Ключ к турниру: '" + m_key + "'");
    m_participants = snapshot.participants;
    m_match_results = snapshot.match_results;
    update_places_lock_held();
}

tournament_handler &tournament_handler::instance() {
    static tournament_handler handler;
    return handler;
}

}  // namespace war_of_ages::client