#include "../include/sprite_printer.h"
#include <TGUI/Widgets/BitmapButton.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "../include/bot.h"
#include "../include/client.h"
#include "../include/game_object_size_constants.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

static void print_units(sf::RenderWindow *window,
                        const std::deque<unit> &units,
                        sprite_supplier::player_side side) {
    sf::Sprite unit_picture;
    for (auto unit : units) {
        sf::RectangleShape unit_hp_in, unit_hp_out;
        if (side == sprite_supplier::player_side::LEFT)
            unit_hp_in.setFillColor({0, 255, 0, 255});
        else
            unit_hp_in.setFillColor({255, 0, 0, 255});

        float x_pos = unit.position() + TOWER_WIDTH, y_pos, hp_len;
        if (unit.type() == unit_type::STONE_TOWER)
            x_pos -= DELTA;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
        }

        if (unit.type() == unit_type::STONE_TOWER) {
            hp_len = TOWER_WIDTH * 0.6;
            y_pos = BACKGROUND_HEIGHT - TOWER_HEIGHT - 10;
            unit_picture = sprite_supplier::get_instance().get_tower_sprite(age_type::STONE, side);
            float hp_x = DELTA;
            if (side == sprite_supplier::player_side::RIGHT) {
                hp_x = ROAD_WIDTH - 1.5 * hp_x;  // Strange constant
            }
            unit_hp_out.setPosition(hp_x, y_pos - DELTA - hp_len);
            unit_hp_in.setPosition(hp_x,
                                   y_pos - DELTA - hp_len * unit.remaining_hp() / unit.stats().initial_hp);
            unit_hp_out.setSize({HP_HEIGHT, hp_len});
            unit_hp_in.setSize({HP_HEIGHT, hp_len * unit.remaining_hp() / unit.stats().initial_hp});
            unit_picture.setPosition(x_pos, y_pos);
        } else {
            y_pos = BACKGROUND_HEIGHT - DELTA - unit.stats().size.y;
            unit_picture = sprite_supplier::get_instance().get_unit_sprite(unit, side);
            hp_len = unit.stats().size.x * 0.7;
            float delta = unit.stats().size.x;
            if (side == sprite_supplier::player_side::RIGHT) {
                delta = 0;
            }
            unit_hp_out.setPosition(x_pos + 0.15 * unit.stats().size.x - delta, y_pos - DELTA);
            unit_hp_in.setPosition(x_pos + 0.15 * unit.stats().size.x - delta, y_pos - DELTA);
            unit_hp_out.setSize({hp_len, HP_HEIGHT});
            unit_hp_in.setSize({hp_len * unit.remaining_hp() / unit.stats().initial_hp, HP_HEIGHT});
            unit_picture.setPosition(x_pos, y_pos);
        }

        window->draw(unit_picture);
        window->draw(unit_hp_out);
        window->draw(unit_hp_in);
    }
}

static void print_bullets(sf::RenderWindow *window,
                          const std::vector<bullet> &bullets,
                          sprite_supplier::player_side side) {
    sf::Sprite bullet_picture;
    for (auto bullet : bullets) {
        bullet_picture = sprite_supplier::get_instance().get_bullet_sprite(bullet.type(), side);

        float x_pos = bullet.pos().x + TOWER_WIDTH,
              y_pos = BACKGROUND_HEIGHT - DELTA - bullet.pos().y - bullet.stats().size.y;
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
    sf::Sprite cannon_picture, cannon_slot_picture;
    for (int i = 0; i < cannons.size(); i++) {
        auto cannon = cannons[i];
        cannon_picture = sprite_supplier::get_instance().get_cannon_sprite(cannon.type(), side);
        cannon_slot_picture =
            sprite_supplier::get_instance().get_cannon_slot_sprite({age_type::STONE, i + 1}, side);

        float x_pos = TOWER_WIDTH + CANNONS_SLOTS_COORD_X[i], y_pos = CANNONS_SLOTS_COORD_Y[i] + DELTA;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
        }

        cannon_picture.setPosition(x_pos, BACKGROUND_HEIGHT - y_pos);
        cannon_slot_picture.setPosition(x_pos, BACKGROUND_HEIGHT - y_pos);
        window->draw(cannon_slot_picture);
        window->draw(cannon_picture);
    }
}

void print(tgui::Gui &gui, sf::RenderWindow *window, const std::shared_ptr<game_state> &state) {
    if (state->get_bot_status(0))
        generate_actions(0, state->snapshot_players().first);
    if (state->get_bot_status(1))
        generate_actions(1, state->snapshot_players().second);
    state->update(current_state.get_player_actions()[0], current_state.get_player_actions()[1],
                  1.f * clock() / CLOCKS_PER_SEC);
    current_state.clear_actions();
    auto [p1, p2] = state->snapshot_players();

    auto background = sprite_supplier::get_instance().get_background_sprite(age_type::STONE);
    background.setPosition(window->getView().getCenter().x - BACKGROUND_WIDTH / 2, 0);
    window->draw(background);

    gui.get(current_state.get_cur_screen_id())
        ->cast<tgui::Group>()
        ->get("coin_label")
        ->cast<tgui::Label>()
        ->setText(std::to_string(p1.money));

    for (int i = 0; i < CANNONS_PER_AGE; i++) {
        auto label = gui.get(current_state.get_cur_screen_id())
                         ->cast<tgui::Group>()
                         ->get("sell_cannon_" + std::to_string(i))
                         ->cast<tgui::Group>()
                         ->get("coin_label")
                         ->cast<tgui::Label>();
        if (i < p1.cannons.size()) {
            label->setText('+' + std::to_string(p1.cannons[i].stats().cost));
        } else {
            label->setText("+0");
        }
    }

    gui.get(current_state.get_cur_screen_id())
        ->cast<tgui::Group>()
        ->get("plus_place_cannon_coin_label")
        ->cast<tgui::Label>()
        ->setText(std::to_string(CANNONS_SLOTS_COSTS[p1.cannons.size()]));

    sf::RectangleShape queued_unit_in, queued_unit_out;
    queued_unit_in.setFillColor(sf::Color::Green);
    float x_pos;
    std::unordered_map<unit_type, float> number_of_units_in_queue = {
        {unit_type::PEASANT, 0}, {unit_type::ARCHER, 0}, {unit_type::CHARIOT, 0}};
    for (int i = 0; i < p1.units_to_train.size(); i++) {
        auto unit = p1.units_to_train[i];
        queued_unit_out.setFillColor(sf::Color::White);
        queued_unit_out.setSize({BUTTON_WIDTH, HP_HEIGHT});
        x_pos = BACKGROUND_WIDTH - 7 * DELTA_X;
        if (unit.type() == unit_type::ARCHER)
            x_pos -= DELTA_X;
        if (unit.type() == unit_type::CHARIOT)
            x_pos -= 2 * DELTA_X;

        queued_unit_out.setPosition(
            {x_pos + (window->getView().getCenter().x - BACKGROUND_WIDTH / 2),
             BUTTON_HEIGHT + BUTTON_Y + (2 * (number_of_units_in_queue[unit.type()]++) + 1) * HP_HEIGHT});
        window->draw(queued_unit_out);

        if (i == 0) {
            queued_unit_in.setSize(
                {BUTTON_WIDTH *
                     (1 - p1.m_training_time_left / p1.units_to_train.front().stats().time_to_train_s),
                 HP_HEIGHT});
            queued_unit_in.setPosition({x_pos + (window->getView().getCenter().x - BACKGROUND_WIDTH / 2),
                                        BUTTON_HEIGHT + BUTTON_Y + HP_HEIGHT});
            window->draw(queued_unit_in);
        }
    }

    auto road = sprite_supplier::get_instance().get_road_sprite(age_type::STONE);
    road.setPosition(0, BACKGROUND_HEIGHT - ROAD_HEIGHT);
    window->draw(road);
    print_units(window, p1.units, sprite_supplier::player_side::LEFT);
    print_units(window, p2.units, sprite_supplier::player_side::RIGHT);
    print_bullets(window, p1.bullets, sprite_supplier::player_side::LEFT);
    print_bullets(window, p2.bullets, sprite_supplier::player_side::RIGHT);
    print_cannons(window, p1.cannons, sprite_supplier::player_side::LEFT);
    print_cannons(window, p2.cannons, sprite_supplier::player_side::RIGHT);
}
}  // namespace war_of_ages