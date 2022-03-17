#include "../include/sprite_supplier.h"
#include "../../game_logic/include/age.h"
#include "../../game_logic/include/bullet.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/game_constants.h"
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
    const static std::vector</*age_type,*/ std::string> background_texture_file{
        /*{age_type::STONE, */ "../client/resources/game/backgrounds/stone/background.png" /*}*/};

    const static std::vector</*age_type,*/ std::string> road_texture_file{
        /*{age_type::STONE, */ "../client/resources/game/roads/stone/road.png" /*}*/};

    const static std::vector</*age_type,*/ std::string> tower_texture_file{
        /*{age_type::STONE,*/ "../client/resources/game/towers/stone/tower.png" /*}*/};

    const static std::vector</*unit_type,*/ std::string> unit_texture_file{
        /*{unit_type::PEASANT,*/ "../client/resources/game/units/stone/peasant.png" /*}*/,
        /*{unit_type::ARCHER,*/ "../client/resources/game/units/stone/archer.png" /*}*/,
        /*{unit_type::CHARIOT,*/ "../client/resources/game/units/stone/chariot.png" /*}*/};

    const static std::vector</*cannon_type,*/ std::string> cannon_texture_file{
        /*{cannon_type::STONE_LEVEL_1,*/ "../client/resources/game/cannons/stone/level_1.png" /*}*/,
        /*{cannon_type::STONE_LEVEL_2,*/ "../client/resources/game/cannons/stone/level_2.png" /*}*/,
        /*{cannon_type::STONE_LEVEL_3,*/ "../client/resources/game/cannons/stone/level_3.png" /*}*/};

    const static std::vector</*std::pair<age_type, int>,*/ std::string> cannon_slots_texture_file{
        /*{{age_type::STONE, 1},*/ "../client/resources/game/cannon_slots/stone/level_1.png" /*}*/,
        /*{{age_type::STONE, 2},*/ "../client/resources/game/cannon_slots/stone/level_2.png" /*}*/,
        /*{{age_type::STONE, 3},*/ "../client/resources/game/cannon_slots/stone/level_3.png" /*}*/};

    const static std::vector</*bullet_type,*/ std::string> bullet_texture_file{
        /*{bullet_type::STONE_LEVEL_1,*/ "../client/resources/game/bullets/stone/level_1.png" /*}*/,
        /*{bullet_type::STONE_LEVEL_2,*/ "../client/resources/game/bullets/stone/level_2.png" /*}*/,
        /*{bullet_type::STONE_LEVEL_3,*/ "../client/resources/game/bullets/stone/level_3.png" /*}*/,
        /*{bullet_type::STONE_ULT,*/ "../client/resources/game/bullets/stone/ult.png" /*}*/};

    background_sprite.resize(background_texture_file.size());
    road_sprite.resize(road_texture_file.size());
    tower_sprite.resize(tower_texture_file.size());
    cannon_sprite.resize(cannon_texture_file.size());
    cannon_slots_sprite.resize(cannon_slots_texture_file.size());
    unit_sprite.resize(unit_texture_file.size());
    bullet_sprite.resize(bullet_texture_file.size());

    for (std::size_t i = 0; i < background_texture_file.size(); i++) {
        background_sprite[i] =
            create_sprite_instance(background_texture_file[i], BACKGROUND_WIDTH, BACKGROUND_HEIGHT);
    }

    for (std::size_t i = 0; i < road_texture_file.size(); i++) {
        road_sprite[i] = create_sprite_instance(road_texture_file[i], ROAD_WIDTH, ROAD_HEIGHT);
    }

    for (std::size_t i = 0; i < tower_texture_file.size(); i++) {
        tower_sprite[i] = create_sprite_instance(tower_texture_file[i], TOWER_WIDTH, TOWER_HEIGHT);
    }

    for (std::size_t i = 0; i < unit_texture_file.size(); i++) {
        vec2f size = unit::get_stats(static_cast<unit_type>(i)).size;
        unit_sprite[i] = create_sprite_instance(unit_texture_file[i], size.x, size.y);
    }

    for (std::size_t i = 0; i < cannon_texture_file.size(); i++) {
        cannon_sprite[i] = create_sprite_instance(cannon_texture_file[i], CANNON_WIDTH, CANNON_HEIGHT);
    }

    for (std::size_t i = 0; i < cannon_slots_texture_file.size(); i++) {
        cannon_slots_sprite[i] =
            create_sprite_instance(cannon_slots_texture_file[i], CANNON_SLOT_WIDTH, CANNON_SLOT_HEIGHT);
    }

    for (std::size_t i = 0; i < bullet_texture_file.size(); i++) {
        vec2f size = bullet::get_stats(static_cast<bullet_type>(i)).size;
        bullet_sprite[i] = create_sprite_instance(bullet_texture_file[i], static_cast<int>(size.x),
                                                  static_cast<int>(size.y));
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
    return background_sprite[static_cast<int>(a_type)];
}

sf::Sprite sprite_supplier::get_road_sprite(age_type a_type) {
    return road_sprite[static_cast<int>(a_type)];
}

sf::Sprite sprite_supplier::get_tower_sprite(age_type a_type, sprite_supplier::player_side side) {
    return reflect_if_needed(tower_sprite[static_cast<int>(a_type)], side);
}

sf::Sprite sprite_supplier::get_cannon_slot_sprite(std::pair<age_type, int> cs_type,
                                                   sprite_supplier::player_side side) {
    return reflect_if_needed(
        cannon_slots_sprite[static_cast<int>(cs_type.first) * CANNONS_PER_AGE + cs_type.second], side);
}

sf::Sprite sprite_supplier::get_unit_sprite(unit_type u_type, sprite_supplier::player_side side) {
    return reflect_if_needed(unit_sprite[static_cast<int>(u_type)], side);
}

sf::Sprite sprite_supplier::get_cannon_sprite(cannon_type c_type, sprite_supplier::player_side side) {
    return reflect_if_needed(cannon_sprite[static_cast<int>(c_type)], side);
}

sf::Sprite sprite_supplier::get_bullet_sprite(bullet_type b_type) {
    return bullet_sprite[static_cast<int>(b_type)];
}

sprite_supplier::~sprite_supplier() {
    for (auto &sprite : background_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : road_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : tower_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : unit_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : cannon_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : cannon_slots_sprite) {
        delete sprite.getTexture();
    }

    for (auto &sprite : bullet_sprite) {
        delete sprite.getTexture();
    }
}

}  // namespace war_of_ages
