#include "../include/sfml_printer.h"
#include <TGUI/Widgets/BitmapButton.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <utility>
#include "../include/client.h"
#include "../include/game_object_size_constants.h"
#include "../include/screen_handler.h"
#include "../include/screens/game_screen.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

static void print_units(sf::RenderWindow *window,
                        const player_snapshot &p,
                        sprite_supplier::player_side side) {
    sf::Sprite unit_picture;
    for (auto unit : p.units) {
        sf::RectangleShape unit_hp_in, unit_hp_out;
        if (side == sprite_supplier::player_side::LEFT)
            unit_hp_in.setFillColor({0, 255, 0, 255});
        else
            unit_hp_in.setFillColor({255, 0, 0, 255});

        float x_pos = unit.position() + TOWER_WIDTH, y_pos, hp_len;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
        }

        if (unit.type() >= unit_type::STONE_TOWER) {
            y_pos = BACKGROUND_HEIGHT - TOWER_HEIGHT;
            unit_picture = sprite_supplier::get_instance().get_tower_sprite(p.age, p.cannons.size(), side);
            unit_picture.setPosition(x_pos, y_pos);
            window->draw(unit_picture);
            continue;
        } else {
            y_pos = BACKGROUND_HEIGHT - Y_COORD_DELTA - unit.stats().size.y;
            unit_picture = sprite_supplier::get_instance().get_unit_sprite(unit, side);
            hp_len = unit.stats().size.x * 0.7;
            float delta = unit.stats().size.x;
            if (side == sprite_supplier::player_side::RIGHT) {
                delta = 0;
            }
            unit_hp_out.setPosition(x_pos + 0.15 * unit.stats().size.x - delta, y_pos - Y_COORD_DELTA);
            unit_hp_in.setPosition(x_pos + 0.15 * unit.stats().size.x - delta, y_pos - Y_COORD_DELTA);
            unit_hp_out.setSize({hp_len, HP_HEIGHT});
            unit_hp_in.setSize({hp_len * unit.remaining_hp() / unit.stats().initial_hp, HP_HEIGHT});
            unit_picture.setPosition(x_pos, y_pos);
        }

        window->draw(unit_picture);
        window->draw(unit_hp_out);
        window->draw(unit_hp_in);
    }
}

static void print_tower_front(sf::RenderWindow *window,
                              const player_snapshot &p,
                              sprite_supplier::player_side side) {
    sf::Sprite tower_front =
        sprite_supplier::get_instance().get_tower_front_sprite(p.age, p.cannons.size(), side);

    sf::RectangleShape unit_hp_in, unit_hp_out;
    if (side == sprite_supplier::player_side::LEFT)
        unit_hp_in.setFillColor({0, 255, 0, 255});
    else
        unit_hp_in.setFillColor({255, 0, 0, 255});

    auto unit = p.units[0];

    float x_pos = unit.position() + TOWER_WIDTH, y_pos, hp_len;
    if (side == sprite_supplier::player_side::RIGHT) {
        x_pos = ROAD_WIDTH - x_pos;
    }

    hp_len = TOWER_WIDTH * 0.6;
    y_pos = BACKGROUND_HEIGHT - TOWER_HEIGHT;
    float hp_x = DELTA;
    if (side == sprite_supplier::player_side::RIGHT) {
        hp_x = ROAD_WIDTH - 1.5 * hp_x;  // Strange constant
    }
    unit_hp_out.setPosition(hp_x, y_pos - Y_COORD_DELTA + static_cast<float>(TOWER_HEIGHT) / 2 - hp_len);
    unit_hp_in.setPosition(hp_x, y_pos - Y_COORD_DELTA + static_cast<float>(TOWER_HEIGHT) / 2 -
                                     hp_len * unit.remaining_hp() / unit.stats().initial_hp);
    unit_hp_out.setSize({HP_HEIGHT, hp_len});
    unit_hp_in.setSize({HP_HEIGHT, hp_len * unit.remaining_hp() / unit.stats().initial_hp});
    tower_front.setPosition(x_pos, y_pos);

    window->draw(tower_front);
    window->draw(unit_hp_out);
    window->draw(unit_hp_in);
}

static void print_bullets(sf::RenderWindow *window,
                          const std::vector<bullet> &bullets,
                          sprite_supplier::player_side side) {
    sf::Sprite bullet_picture;
    for (auto bullet : bullets) {
        bullet_picture = sprite_supplier::get_instance().get_bullet_sprite(bullet.type(), side);

        float x_pos = bullet.pos().x + TOWER_WIDTH,
              y_pos = BACKGROUND_HEIGHT - Y_COORD_DELTA - bullet.pos().y - bullet.stats().size.y;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
        }

        bullet_picture.setPosition(x_pos, y_pos);
        window->draw(bullet_picture);
    }
}

static void print_cannons(sf::RenderWindow *window,
                          const std::vector<cannon> &cannons,
                          sprite_supplier::player_side side) {
    sf::Sprite cannon_picture;
    for (int i = 0; i < cannons.size(); i++) {
        auto cannon = cannons[i];
        cannon_picture = sprite_supplier::get_instance().get_cannon_sprite(cannon.type(), side);

        float x_pos = TOWER_WIDTH + CANNONS_SLOTS_COORD_X[i],
              y_pos = CANNONS_SLOTS_COORD_Y[i] + Y_COORD_DELTA + CANNON_HEIGHT;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
        }

        cannon_picture.setPosition(x_pos, BACKGROUND_HEIGHT - y_pos);
        window->draw(cannon_picture);
    }
}

void sfml_printer::print_game(const std::pair<player_snapshot, player_snapshot> &snapshots) {
    auto [p1, p2] = snapshots;
    auto background = sprite_supplier::get_instance().get_background_sprite(p1.age);
    background.setPosition(m_window.getView().getCenter().x - BACKGROUND_WIDTH / 2, 0);
    m_window.draw(background);

    auto &gui = screen_handler::instance().get_gui();  // TODO: think whether it should be here

    gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))
        ->cast<tgui::Group>()
        ->get("coin_label")
        ->cast<tgui::Label>()
        ->setText(std::to_string(p1.money));

    gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))
        ->cast<tgui::Group>()
        ->get("exp_label")
        ->cast<tgui::Label>()
        ->setText(std::to_string(p1.exp));

    for (int i = 0; i < CANNONS_PER_AGE; i++) {
        auto label = gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))
                         ->cast<tgui::Group>()
                         ->get("sell_cannon_" + std::to_string(i))
                         ->cast<tgui::Group>()
                         ->get("coin_label")
                         ->cast<tgui::Label>();
        if (i < p1.cannons.size()) {
            label->setText('+' + std::to_string(p1.cannons[i].stats().cost / 2));
        } else {
            label->setText("+0");
        }
    }

    gui.get(screen_handler::screen_id.at(screen_handler::screen_type::GAME_SCREEN))
        ->cast<tgui::Group>()
        ->get("plus_place_cannon_coin_label")
        ->cast<tgui::Label>()
        ->setText('-' + std::to_string(CANNONS_SLOTS_COSTS[p1.cannons.size()]));

    sf::RectangleShape queued_unit_in, queued_unit_out;
    queued_unit_in.setFillColor(sf::Color::Green);
    float x_pos;
    std::vector<float> number_of_units_in_queue = {0, 0, 0};
    for (int i = 0; i < p1.units_to_train.size(); i++) {
        auto unit = p1.units_to_train[i];
        queued_unit_out.setFillColor(sf::Color::White);
        queued_unit_out.setSize({BUTTON_WIDTH, HP_HEIGHT});
        int unit_no = static_cast<int>(unit.type()) - 3 * static_cast<int>(p1.age);
        if (unit_no < 0)
            unit_no += (abs(unit_no / 3) + 1) * 3;
        x_pos = BACKGROUND_WIDTH - 7 * DELTA_X - unit_no * DELTA_X;

        queued_unit_out.setPosition(
            {x_pos + (m_window.getView().getCenter().x - BACKGROUND_WIDTH / 2),
             BUTTON_HEIGHT + BUTTON_Y + (2 * (number_of_units_in_queue[unit_no]++) + 1) * HP_HEIGHT});
        m_window.draw(queued_unit_out);

        if (i == 0) {
            queued_unit_in.setSize(
                {BUTTON_WIDTH *
                     (1 - p1.m_training_time_left / p1.units_to_train.front().stats().time_to_train_s),
                 HP_HEIGHT});
            queued_unit_in.setPosition({x_pos + (m_window.getView().getCenter().x - BACKGROUND_WIDTH / 2),
                                        BUTTON_HEIGHT + BUTTON_Y + HP_HEIGHT});
            m_window.draw(queued_unit_in);
        }
    }

    sf::RectangleShape ult_in, ult_out;
    ult_out.setFillColor(sf::Color::Green);
    ult_in.setFillColor(sf::Color::White);
    ult_out.setSize({BUTTON_WIDTH * 4, HP_HEIGHT});
    ult_in.setSize({p1.m_ult_cooldown * BUTTON_WIDTH * 4 / ULT_COOLDOWN, HP_HEIGHT});
    ult_out.setPosition(
        BACKGROUND_WIDTH - 3 * DELTA_X + (m_window.getView().getCenter().x - BACKGROUND_WIDTH / 2),
        2 * (BUTTON_HEIGHT + BUTTON_Y));
    ult_in.setPosition(BACKGROUND_WIDTH - 3 * DELTA_X +
                           (ULT_COOLDOWN - p1.m_ult_cooldown) * BUTTON_WIDTH * 4 / ULT_COOLDOWN +
                           (m_window.getView().getCenter().x - BACKGROUND_WIDTH / 2),
                       2 * (BUTTON_HEIGHT + BUTTON_Y));
    m_window.draw(ult_out);
    m_window.draw(ult_in);

    auto road = sprite_supplier::get_instance().get_road_sprite(p1.age);
    road.setPosition(0, BACKGROUND_HEIGHT - ROAD_HEIGHT);
    m_window.draw(road);
    print_units(&m_window, p1, sprite_supplier::player_side::LEFT);
    print_units(&m_window, p2, sprite_supplier::player_side::RIGHT);
    print_tower_front(&m_window, p1, sprite_supplier::player_side::LEFT);
    print_tower_front(&m_window, p2, sprite_supplier::player_side::RIGHT);
    print_bullets(&m_window, p1.bullets, sprite_supplier::player_side::LEFT);
    print_bullets(&m_window, p2.bullets, sprite_supplier::player_side::RIGHT);
    print_cannons(&m_window, p1.cannons, sprite_supplier::player_side::LEFT);
    print_cannons(&m_window, p2.cannons, sprite_supplier::player_side::RIGHT);
}

void sfml_printer::handle_window_events() {
    static float prev_x;
    static bool moving = false;

    sf::Event event{};
    while (m_window.pollEvent(event)) {
        screen_handler::instance().get_gui().handleEvent(event);
        switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == 0) {
                    moving = true;
                    prev_x = event.mouseButton.x;
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0) {
                    moving = false;
                }
                break;
            case sf::Event::MouseMoved: {
                if (!moving ||
                    screen_handler::instance().get_screen_type() != screen_handler::screen_type::GAME_SCREEN)
                    break;

                float delta = prev_x - event.mouseMove.x;
                if (m_view.getCenter().x + delta < 1.f * BACKGROUND_WIDTH / 2) {
                    delta = 1.f * BACKGROUND_WIDTH / 2 - m_view.getCenter().x;
                }
                if (m_view.getCenter().x + delta > ROAD_WIDTH - 1.f * BACKGROUND_WIDTH / 2) {
                    delta = ROAD_WIDTH - 1.f * BACKGROUND_WIDTH / 2 - m_view.getCenter().x;
                }
                m_view.move(delta, 0.0f);

                prev_x = event.mouseMove.x;
                break;
            }
            default:
                break;
        }
    }
}

sfml_printer &sfml_printer::instance() {
    static sfml_printer printer;
    return printer;
}

sfml_printer::sfml_printer()
    : m_window(sf::VideoMode::getFullscreenModes()[0], "War of Ages", sf::Style::Fullscreen), m_view() {
}

void sfml_printer::init() {
    m_window.setVerticalSyncEnabled(true);
    m_view = m_window.getDefaultView();
}

sf::View &sfml_printer::get_view() {
    return m_view;
}

sf::RenderWindow &sfml_printer::get_window() {
    return m_window;
}

void sfml_printer::set_view_center(int x, int y) {
    m_view.setCenter(x, y);
}

}  // namespace war_of_ages