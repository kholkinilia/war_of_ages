#ifndef WAR_OF_AGES_SPRITE_SUPPLIER_H
#define WAR_OF_AGES_SPRITE_SUPPLIER_H

#include "../../game_logic/include/age.h"
#include "../../game_logic/include/unit.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/bullet.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>

namespace war_of_ages {

// TODO: make different suppliers for different types (maybe?)
struct sprite_supplier {
private:

    static std::unordered_map<age_type, sf::Sprite> background_sprite;
    static std::unordered_map<age_type, sf::Sprite> road_sprite;
    static std::unordered_map<age_type, sf::Sprite> tower_sprite;
    static std::map<std::pair<age_type, int>, sf::Sprite> cannon_slots_sprite; // TODO: add hash
    static std::unordered_map<unit_type, sf::Sprite> unit_sprite;
    static std::unordered_map<cannon_type, sf::Sprite> cannon_sprite;
    static std::unordered_map<bullet_type, sf::Sprite> bullet_sprite;

    sprite_supplier();

public:
    enum class direction {
        LEFT,
        RIGHT
    };

    sf::Sprite get_background_sprite(age_type a_type);
    sf::Sprite get_road_sprite(age_type a_type);
    sf::Sprite get_tower_sprite(age_type a_type, direction dir);
    sf::Sprite get_cannon_slot_sprite(age_type a_type, direction dir);
    sf::Sprite get_unit_sprite(unit_type u_type, direction dir);
    sf::Sprite get_cannon_sprite(cannon_type c_type, direction dir);
    sf::Sprite get_bullet_sprite(bullet_type b_type, direction dir);

    sprite_supplier(const sprite_supplier&) = delete;
    sprite_supplier(sprite_supplier &&) = default;
    sprite_supplier &operator=(const sprite_supplier&) = delete;
    sprite_supplier &operator=(sprite_supplier&&) = default;

    ~sprite_supplier() = default;

    static sprite_supplier* get_instance();
};


}

#endif  // WAR_OF_AGES_SPRITE_SUPPLIER_H
