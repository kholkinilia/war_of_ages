#ifndef WAR_OF_AGES_SPRITE_SUPPLIER_H
#define WAR_OF_AGES_SPRITE_SUPPLIER_H

#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../game_logic/include/age.h"
#include "../../game_logic/include/bullet.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/unit.h"

namespace war_of_ages {

// TODO: make different suppliers for different types (maybe?)
struct sprite_supplier {
public:
    enum class player_side { LEFT, RIGHT };

    sf::Sprite get_background_sprite(age_type a_type);
    sf::Sprite get_road_sprite(age_type a_type);
    sf::Sprite get_tower_sprite(age_type a_type, player_side side);
    sf::Sprite get_cannon_slot_sprite(std::pair<age_type, int> cs_type, player_side side);
    sf::Sprite get_unit_sprite(unit_type u_type, player_side side);
    sf::Sprite get_cannon_sprite(cannon_type c_type, player_side side);
    sf::Sprite get_bullet_sprite(bullet_type b_type);

    sprite_supplier(const sprite_supplier &) = delete;
    sprite_supplier(sprite_supplier &&) = default;
    sprite_supplier &operator=(const sprite_supplier &) = delete;
    sprite_supplier &operator=(sprite_supplier &&) = default;

    ~sprite_supplier();

    [[nodiscard]] static sprite_supplier &get_instance();

private:
    std::vector</*age_type, */ sf::Sprite> background_sprite;
    std::vector</*age_type, */ sf::Sprite> road_sprite;
    std::vector</*age_type, */ sf::Sprite> tower_sprite;
    std::vector</*std::pair<age_type, int>, */ sf::Sprite> cannon_slots_sprite;
    std::vector</*unit_type, */ sf::Sprite> unit_sprite;
    std::vector</*cannon_type, */ sf::Sprite> cannon_sprite;
    std::vector</*bullet_type, */ sf::Sprite> bullet_sprite;

    sprite_supplier();

    static sf::Sprite create_sprite_instance(const std::string &filename, int width, int height);
    static sf::Sprite reflect_if_needed(sf::Sprite sprite, player_side side);
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SPRITE_SUPPLIER_H
