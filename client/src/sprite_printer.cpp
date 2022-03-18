#include "../include/sprite_printer.h"
#include "../include/game_object_size_constants.h"

namespace war_of_ages {
sprite_printer::sprite_printer() {
    background = sprite_supplier::get_instance().get_background_sprite(age_type::STONE);
}

void sprite_printer::update(float delta) {
    sf::Vector2f deltaPos = {delta, 0.0};
    background.setPosition(background.getPosition() - deltaPos);
}

void sprite_printer::print(sf::RenderWindow *window, const std::shared_ptr<game_state> &state) {
    state->update({}, {}, 1.f * clock() / CLOCKS_PER_SEC);
    auto [p1, p2] = state->snapshot_players();

    background.setPosition(0, 0);
    window->draw(background);

    auto road = sprite_supplier::get_instance().get_road_sprite(age_type::STONE);
    road.setPosition(0, BACKGROUND_HEIGHT - ROAD_HEIGHT);
    window->draw(road);
    print_units(window, p1.units, sprite_supplier::player_side::LEFT);
    print_units(window, p2.units, sprite_supplier::player_side::RIGHT);
}

void sprite_printer::print_units(sf::RenderWindow *window,
                                 const std::deque<unit> &units,
                                 sprite_supplier::player_side side) {
    sf::Sprite unit_picture;
    for (auto unit : units) {
        sf::RectangleShape unit_hp_in, unit_hp_out;
        if (side == sprite_supplier::player_side::LEFT)
            unit_hp_in.setFillColor({0, 255, 0, 255});
        else
            unit_hp_in.setFillColor({255, 0, 0, 255});

        float x_pos = unit.position() * (ROAD_WIDTH - 2 * (TOWER_WIDTH - 2 * DELTA)) / FIELD_LENGTH_PXLS +
                      TOWER_WIDTH - DELTA,
              y_pos, hp_len, delta = unit.stats().size.x;
        if (side == sprite_supplier::player_side::RIGHT) {
            x_pos = ROAD_WIDTH - x_pos;
            delta = 0;
        }

        if (unit.type() == unit_type::STONE_TOWER) {
            if (side == sprite_supplier::player_side::LEFT) {
                delta = TOWER_WIDTH - DELTA - 10;
            } else {
                delta = -0.4 * TOWER_WIDTH + DELTA + 10;
            }
            y_pos = BACKGROUND_HEIGHT - TOWER_HEIGHT - 10;
            unit_picture = sprite_supplier::get_instance().get_tower_sprite(age_type::STONE, side);
            hp_len = TOWER_WIDTH * 0.6;
            unit_hp_out.setPosition(x_pos - delta, y_pos - DELTA);
            unit_hp_in.setPosition(x_pos - delta, y_pos - DELTA);
        } else {
            y_pos = BACKGROUND_HEIGHT - DELTA - unit.stats().size.y;
            unit_picture = sprite_supplier::get_instance().get_unit_sprite(unit.type(), side);
            hp_len = unit.stats().size.x * 0.7;
            unit_hp_out.setPosition(x_pos - delta + 0.15 * unit.stats().size.x, y_pos - DELTA);
            unit_hp_in.setPosition(x_pos - delta + 0.15 * unit.stats().size.x, y_pos - DELTA);
        }

        unit_hp_out.setSize({hp_len, HP_HEIGHT});
        unit_hp_in.setSize({hp_len * unit.remaining_hp() / unit.stats().initial_hp, HP_HEIGHT});
        unit_picture.setPosition(x_pos, y_pos);
        window->draw(unit_picture);
        window->draw(unit_hp_out);
        window->draw(unit_hp_in);
    }
}

}  // namespace war_of_ages