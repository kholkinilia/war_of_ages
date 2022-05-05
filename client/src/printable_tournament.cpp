#include "../include/printable_tournament.h"
#include <TGUI/Widgets/Label.hpp>

namespace war_of_ages {

void printable_tournament::update_grid(const tgui::Grid::Ptr &grid) {
    // TODO: think of improving performance (should be easy)
    std::unique_lock lock(m);
    if (is_grid_updated) {
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

    is_grid_updated = true;
}

}  // namespace war_of_ages