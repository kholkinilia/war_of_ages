#include "../include/sprite_supplier.h"
#include "../../game_logic/include/age.h"
#include "../../game_logic/include/unit.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/bullet.h"
#include "../include/game_object_size_constants.h"

namespace war_of_ages {

sprite_supplier::sprite_supplier() {
    const static std::unordered_map<age_type, std::string> background_texture_file{
        {age_type::STONE, "../resources/game/backgrounds/stone/backgrounds.png"}};
    const static std::unordered_map<age_type, std::string> road_texture_file{
        {age_type::STONE, "../resources/game/roads/stone/road.png"}};
    const static std::unordered_map<age_type, std::string> tower_texture_file{
        {age_type::STONE, "../resources/game/towers/stone/tower.png"}};
    const static std::unordered_map<unit_type, std::string> unit_texture_file{
        {unit_type::PEASANT, "../resources/game/towers/stone/peasant.png"},
        {unit_type::ARCHER, "../resources/game/towers/stone/archer.png"},
        {unit_type::CHARIOT, "../resources/game/towers/stone/chariot.png"}};
    const static std::unordered_map<cannon_type, std::string> cannon_texture_file{
        {cannon_type::STONE_LEVEL_1, "../resources/game/cannons/stone/level_1.png"},
        {cannon_type::STONE_LEVEL_2, "../resources/game/cannons/stone/level_2.png"},
        {cannon_type::STONE_LEVEL_3, "../resources/game/cannons/stone/level_3.png"},
    };
    const static std::map<std::pair<age_type, int>, std::string> cannon_slots_texture_file{
        {{age_type::STONE, 1}, "../resources/game/cannon_slots/stone/level_1.png"},
        {{age_type::STONE, 2}, "../resources/game/cannon_slots/stone/level_2.png"},
        {{age_type::STONE, 3}, "../resources/game/cannon_slots/stone/level_3.png"}};
    const static std::unordered_map<bullet_type, std::string> bullet_texture_file{
        {bullet_type::STONE_LEVEL_1, "../resources/game/bullets/stone/level_1.png"},
        {bullet_type::STONE_LEVEL_2, "../resources/game/bullets/stone/level_2.png"},
        {bullet_type::STONE_LEVEL_3, "../resources/game/bullets/stone/level_3.png"},
        {bullet_type::STONE_ULT, "../resources/game/bullets/stone/ult.png"},
    };
}

sprite_supplier *sprite_supplier::get_instance() {
    static sprite_supplier supplier{};
    return &supplier;
}

sf::Sprite sprite_supplier::get_background_sprite(age_type a_type) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_road_sprite(age_type a_type) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_tower_sprite(age_type a_type, sprite_supplier::direction dir) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_cannon_slot_sprite(age_type a_type, sprite_supplier::direction dir) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_unit_sprite(unit_type u_type, sprite_supplier::direction dir) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_cannon_sprite(cannon_type c_type, sprite_supplier::direction dir) {
    return sf::Sprite();
}

sf::Sprite sprite_supplier::get_bullet_sprite(bullet_type b_type, sprite_supplier::direction dir) {
    return sf::Sprite();
}

}  // namespace war_of_ages
