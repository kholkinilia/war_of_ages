#include "../include/sprite_supplier.h"
#include "../../game_logic/include/age.h"
#include "../../game_logic/include/bullet.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/unit.h"
#include "../include/game_object_size_constants.h"

namespace war_of_ages {

sf::Sprite sprite_supplier::create_sprite_instance(const std::string &filename, int width, int height) {
    auto *texture = new sf::Texture();
    texture->loadFromFile(filename);
    sf::Sprite result(*texture);
    result.setScale(1. * width / texture->getSize().x, 1. * height / texture->getSize().y);
    return result;
}

sprite_supplier::sprite_supplier() {
    const static std::unordered_map<age_type, std::string> background_texture_file{
        {age_type::STONE, "../client/resources/game/backgrounds/stone/background.png"}};

    const static std::unordered_map<age_type, std::string> road_texture_file{
        {age_type::STONE, "../client/resources/game/roads/stone/road.png"}};

    const static std::unordered_map<age_type, std::string> tower_texture_file{
        {age_type::STONE, "../client/resources/game/towers/stone/tower.png"}};

    const static std::unordered_map<unit_type, std::string> unit_texture_file{
        {unit_type::PEASANT, "../client/resources/game/units/stone/peasant_animated.png"},
        {unit_type::ARCHER, "../client/resources/game/units/stone/archer_animated.png"},
        {unit_type::CHARIOT, "../client/resources/game/units/stone/chariot_animated.png"}};

    const static std::unordered_map<unit_type, std::pair<int, int>> animation_size{
        {unit_type::PEASANT, {2, 3}}, {unit_type::ARCHER, {2, 1}}, {unit_type::CHARIOT, {2, 1}}};

    const static std::unordered_map<unit_type, std::vector<float>> animation_time_periods{
        {unit_type::PEASANT, {0.5, 0.5}}, {unit_type::ARCHER, {1, 1}}, {unit_type::CHARIOT, {1.5, 1}}};

    const static std::unordered_map<cannon_type, std::string> cannon_texture_file{
        {cannon_type::STONE_LEVEL_1, "../client/resources/game/cannons/stone/level_1.png"},
        {cannon_type::STONE_LEVEL_2, "../client/resources/game/cannons/stone/level_2.png"},
        {cannon_type::STONE_LEVEL_3, "../client/resources/game/cannons/stone/level_3.png"}};

    const static std::map<std::pair<age_type, int>, std::string> cannon_slots_texture_file{
        {{age_type::STONE, 1}, "../client/resources/game/cannon_slots/stone/level_1.png"},
        {{age_type::STONE, 2}, "../client/resources/game/cannon_slots/stone/level_2.png"},
        {{age_type::STONE, 3}, "../client/resources/game/cannon_slots/stone/level_3.png"}};

    const static std::unordered_map<bullet_type, std::string> bullet_texture_file{
        {bullet_type::STONE_LEVEL_1, "../client/resources/game/bullets/stone/level_1.png"},
        {bullet_type::STONE_LEVEL_2, "../client/resources/game/bullets/stone/level_2.png"},
        {bullet_type::STONE_LEVEL_3, "../client/resources/game/bullets/stone/level_3.png"},
        {bullet_type::STONE_ULT, "../client/resources/game/bullets/stone/ult.png"}};

    for (auto &[a_type, filename] : background_texture_file) {
        background_sprite[a_type] = create_sprite_instance(filename, ROAD_WIDTH, BACKGROUND_HEIGHT);
    }

    for (auto &[a_type, filename] : road_texture_file) {
        road_sprite[a_type] = create_sprite_instance(filename, ROAD_WIDTH, ROAD_HEIGHT);
    }

    for (auto &[a_type, filename] : tower_texture_file) {
        tower_sprite[a_type] = create_sprite_instance(filename, TOWER_WIDTH, TOWER_HEIGHT);
    }

    for (auto &[u_type, filename] : unit_texture_file) {
        auto sz = animation_size.at(u_type);
        unit_sprite.insert(
            {u_type, animation_handler(filename, animation_time_periods.at(u_type), sz.first, sz.second,
                                       static_cast<int>(unit::get_stats(u_type).size.x),
                                       static_cast<int>(unit::get_stats(u_type).size.y))});

        //            create_sprite_instance(filename, static_cast<int>(unit::get_stats(u_type).size.x),
        //                                   static_cast<int>(unit::get_stats(u_type).size.y));
    }

    for (auto &[c_type, filename] : cannon_texture_file) {
        cannon_sprite[c_type] = create_sprite_instance(filename, CANNON_WIDTH, CANNON_HEIGHT);
    }

    for (auto &[cs_type, filename] : cannon_slots_texture_file) {
        cannon_slots_sprite[cs_type] =
            create_sprite_instance(filename, CANNON_SLOT_WIDTH, CANNON_SLOT_HEIGHT);
    }

    for (auto &[b_type, filename] : bullet_texture_file) {
        bullet_sprite[b_type] =
            create_sprite_instance(filename, static_cast<int>(bullet::get_stats(b_type).size.x),
                                   static_cast<int>(bullet::get_stats(b_type).size.y));
    }
}

sf::Sprite sprite_supplier::reflect_if_needed(sf::Sprite sprite, sprite_supplier::player_side side) {
    if (side == player_side::LEFT) {
        sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
    }
    return sprite;
}

sprite_supplier &sprite_supplier::get_instance() {
    static sprite_supplier instance;
    return instance;
}

sf::Sprite sprite_supplier::get_background_sprite(age_type a_type) {
    return background_sprite[a_type];
}

sf::Sprite sprite_supplier::get_road_sprite(age_type a_type) {
    return road_sprite[a_type];
}

sf::Sprite sprite_supplier::get_tower_sprite(age_type a_type, sprite_supplier::player_side side) {
    return reflect_if_needed(tower_sprite[a_type], side);
}

sf::Sprite sprite_supplier::get_cannon_slot_sprite(std::pair<age_type, int> cs_type,
                                                   sprite_supplier::player_side side) {
    return reflect_if_needed(cannon_slots_sprite[cs_type], side);
}

sf::Sprite sprite_supplier::get_unit_sprite(const unit &source_unit, sprite_supplier::player_side side) {
    if (source_unit.is_attacking()) {
        return reflect_if_needed(unit_sprite[source_unit.type()].get_sprite(1, source_unit.attack_progress()),
                                 side);
    }
    return reflect_if_needed(unit_sprite[source_unit.type()].get_sprite(0, source_unit.walking_time()), side);
}

sf::Sprite sprite_supplier::get_cannon_sprite(cannon_type c_type, sprite_supplier::player_side side) {
    return reflect_if_needed(cannon_sprite[c_type], side);
}

sf::Sprite sprite_supplier::get_bullet_sprite(bullet_type b_type) {
    return bullet_sprite[b_type];
}

sprite_supplier::~sprite_supplier() {
    for (auto &[a_type, sprite] : background_sprite) {
        delete sprite.getTexture();
    }

    for (auto &[a_type, sprite] : road_sprite) {
        delete sprite.getTexture();
    }

    for (auto &[a_type, sprite] : tower_sprite) {
        delete sprite.getTexture();
    }

    //    for (auto &[u_type, sprite] : unit_sprite) {
    //        delete sprite.getTexture();
    //    }

    for (auto &[c_type, sprite] : cannon_sprite) {
        delete sprite.getTexture();
    }

    for (auto &[cs_type, sprite] : cannon_slots_sprite) {
        delete sprite.getTexture();
    }

    for (auto &[b_type, sprite] : bullet_sprite) {
        delete sprite.getTexture();
    }
}

}  // namespace war_of_ages