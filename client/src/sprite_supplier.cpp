#include "sprite_supplier.h"
#include "bot_actions_supplier.h"
#include "game_object_size_constants.h"
#include "age.h"
#include "bullet.h"
#include "cannon.h"
#include "unit.h"

namespace war_of_ages::client {

sf::Sprite sprite_supplier::create_sprite_instance(const std::string &filename, int width, int height) {
    auto *texture = new sf::Texture();
    texture->loadFromFile(filename);
    sf::Sprite result(*texture);
    result.setScale(1. * width / texture->getSize().x, 1. * height / texture->getSize().y);
    return result;
}

sprite_supplier::sprite_supplier() {
    std::string common_file_prefix = "../../client/resources/game/";

    static std::unordered_map<age_type, std::string> age_to_string{
        {age_type::STONE, "stone"}, {age_type::CASTLE, "castle"},
        // {age_type::RENAISSANCE, "renaissance"},
        // {age_type::MODERN, "modern"},
        // {age_type::FUTURE, "future"}
    };

    static std::unordered_map<unit_type, std::string> unit_to_string{
        {unit_type::PEASANT, "peasant"},       {unit_type::ARCHER, "archer"},
        {unit_type::CHARIOT, "chariot"},       {unit_type::SWORDSMAN, "swordsman"},
        {unit_type::ARBALESTER, "arbalester"}, {unit_type::KNIGHT, "knight"}};

    static auto get_background_file = [&](age_type type) {
        return common_file_prefix + "backgrounds" + "/" + age_to_string[type] + "/" + "background.png";
    };

    static auto get_road_file = [&](age_type type) {
        return common_file_prefix + "roads" + "/" + age_to_string[type] + "/" + "road.png";
    };

    static auto get_tower_file = [&](age_type type, int number_of_slots) {
        return common_file_prefix + "towers" + "/" + age_to_string[type] + "/" + "tower" +
               std::to_string(number_of_slots) + ".png";
    };

    static auto get_tower_front_file = [&](age_type type, int number_of_slots) {
        return common_file_prefix + "towers" + "/" + age_to_string[type] + "/" + "front" +
               std::to_string(number_of_slots) + ".png";
    };

    static auto get_unit_file = [&](age_type a_type, unit_type u_type) {
        return common_file_prefix + "units" + "/" + age_to_string[a_type] + "/" + unit_to_string[u_type] +
               ".png";
    };

    const static std::unordered_map<unit_type, std::pair<int, int>> animation_size{
        {unit_type::PEASANT, {3, 3}},   {unit_type::ARCHER, {4, 6}},     {unit_type::CHARIOT, {3, 6}},
        {unit_type::SWORDSMAN, {3, 6}}, {unit_type::ARBALESTER, {4, 6}}, {unit_type::KNIGHT, {3, 6}}};

    const static std::unordered_map<unit_type, std::vector<float>> animation_time_periods{
        {unit_type::PEASANT, {0.5, unit::get_stats(unit_type::PEASANT).attack_duration_s, 1}},
        {unit_type::ARCHER,
         {1, unit::get_stats(unit_type::ARCHER).attack_duration_s, 1,
          unit::get_stats(unit_type::ARCHER).attack_duration_s}},
        {unit_type::CHARIOT, {1.5, unit::get_stats(unit_type::CHARIOT).attack_duration_s, 1}},
        {unit_type::SWORDSMAN, {1, unit::get_stats(unit_type::SWORDSMAN).attack_duration_s, 1}},
        {unit_type::ARBALESTER,
         {1, unit::get_stats(unit_type::ARBALESTER).attack_duration_s, 1,
          unit::get_stats(unit_type::ARBALESTER).attack_duration_s}},
        {unit_type::KNIGHT, {1.5, unit::get_stats(unit_type::KNIGHT).attack_duration_s, 1}},
    };

    static std::unordered_map<cannon_type, std::string> cannon_to_string{
        {cannon_type::STONE_LEVEL_1, "level1"},  {cannon_type::STONE_LEVEL_2, "level2"},
        {cannon_type::STONE_LEVEL_3, "level3"},  {cannon_type::CASTLE_LEVEL_1, "level1"},
        {cannon_type::CASTLE_LEVEL_2, "level2"}, {cannon_type::CASTLE_LEVEL_3, "level3"}};

    static auto get_cannon_file = [&](age_type a_type, cannon_type c_type) {
        return common_file_prefix + "cannons" + "/" + age_to_string[a_type] + "/" + cannon_to_string[c_type] +
               ".png";
    };

    static std::unordered_map<bullet_type, std::string> bullet_to_string{
        {bullet_type::STONE_LEVEL_1, "level1"},  {bullet_type::STONE_LEVEL_2, "level2"},
        {bullet_type::STONE_LEVEL_3, "level3"},  {bullet_type::STONE_ULT, "ult"},
        {bullet_type::CASTLE_LEVEL_1, "level1"}, {bullet_type::CASTLE_LEVEL_2, "level2"},
        {bullet_type::CASTLE_LEVEL_3, "level3"}, {bullet_type::CASTLE_ULT, "ult"}};

    static auto get_bullet_file = [&](age_type a_type, bullet_type b_type) {
        return common_file_prefix + "bullets" + "/" + age_to_string[a_type] + "/" + bullet_to_string[b_type] +
               ".png";
    };

    const static std::vector<age_type> drawn_ages{age_type::STONE, age_type::CASTLE};

    const static std::unordered_map<age_type, std::vector<unit_type>> units_by_age{
        {age_type::STONE, {unit_type::PEASANT, unit_type::ARCHER, unit_type::CHARIOT}},
        {age_type::CASTLE, {unit_type::SWORDSMAN, unit_type::ARBALESTER, unit_type::KNIGHT}}};

    const static std::unordered_map<age_type, std::vector<cannon_type>> cannons_by_age{
        {age_type::STONE,
         {cannon_type::STONE_LEVEL_1, cannon_type::STONE_LEVEL_2, cannon_type::STONE_LEVEL_3}},
        {age_type::CASTLE,
         {cannon_type::CASTLE_LEVEL_1, cannon_type::CASTLE_LEVEL_2, cannon_type::CASTLE_LEVEL_3}}};

    const static std::unordered_map<age_type, std::vector<bullet_type>> bullets_by_age{
        {age_type::STONE,
         {bullet_type::STONE_LEVEL_1, bullet_type::STONE_LEVEL_2, bullet_type::STONE_LEVEL_3,
          bullet_type::STONE_ULT}},
        {age_type::CASTLE,
         {bullet_type::CASTLE_LEVEL_1, bullet_type::CASTLE_LEVEL_2, bullet_type::CASTLE_LEVEL_3,
          bullet_type::CASTLE_ULT}}};

    for (auto a_type : drawn_ages) {
        background_sprite[a_type] =
            create_sprite_instance(get_background_file(a_type), ROAD_WIDTH, BACKGROUND_HEIGHT);
        road_sprite[a_type] = create_sprite_instance(get_road_file(a_type), ROAD_WIDTH, ROAD_HEIGHT);
        for (int i = 1; i <= 3; i++) {
            tower_sprite[{a_type, i}] =
                create_sprite_instance(get_tower_file(a_type, i), TOWER_WIDTH, TOWER_HEIGHT);
            tower_front_sprite[{a_type, i}] =
                create_sprite_instance(get_tower_front_file(a_type, i), TOWER_WIDTH, TOWER_HEIGHT);
        }
        for (auto u_type : units_by_age.at(a_type)) {
            auto sz = animation_size.at(u_type);
            unit_sprite.insert(
                {u_type,
                 animation_supplier(get_unit_file(a_type, u_type), animation_time_periods.at(u_type),
                                    sz.first, sz.second, static_cast<int>(unit::get_stats(u_type).size.x),
                                    static_cast<int>(unit::get_stats(u_type).size.y))});
        }
        for (auto c_type : cannons_by_age.at(a_type)) {
            cannon_sprite[c_type] =
                create_sprite_instance(get_cannon_file(a_type, c_type), CANNON_WIDTH, CANNON_HEIGHT);
        }
        for (auto b_type : bullets_by_age.at(a_type)) {
            bullet_sprite[b_type] = create_sprite_instance(
                get_bullet_file(a_type, b_type), static_cast<int>(bullet::get_stats(b_type).size.x),
                static_cast<int>(bullet::get_stats(b_type).size.y));
        }
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

sf::Sprite sprite_supplier::get_tower_sprite(age_type a_type,
                                             std::size_t number_of_slots,
                                             sprite_supplier::player_side side) {
    return reflect_if_needed(tower_sprite[{a_type, number_of_slots}], side);
}

sf::Sprite sprite_supplier::get_tower_front_sprite(age_type a_type,
                                                   std::size_t number_of_slots,
                                                   sprite_supplier::player_side side) {
    return reflect_if_needed(tower_front_sprite[{a_type, number_of_slots}], side);
}

sf::Sprite sprite_supplier::get_cannon_slot_sprite(std::pair<age_type, int> cs_type,
                                                   sprite_supplier::player_side side) {
    return reflect_if_needed(cannon_slots_sprite[cs_type], side);
}

sf::Sprite sprite_supplier::get_unit_sprite(const std::shared_ptr<unit>& source_unit, sprite_supplier::player_side side) {
    const static int WALKING = 0;
    const static int ATTACKING = 1;
    const static int STANDING = 2;
    const static int WALKING_ATTACKING = 3;
    // TODO: avoid copy-paste
    if (source_unit->is_walking()) {
        if (source_unit->is_attacking()) {
            return reflect_if_needed(
                unit_sprite[source_unit->type()].get_sprite(WALKING_ATTACKING, source_unit->attack_progress()),
                side);
        }
        return reflect_if_needed(
            unit_sprite[source_unit->type()].get_sprite(WALKING, source_unit->walking_time()), side);
    }
    if (source_unit->is_attacking()) {
        return reflect_if_needed(
            unit_sprite[source_unit->type()].get_sprite(ATTACKING, source_unit->attack_progress()), side);
    }
    // FIXME: replace attack_progress() with lifetime when implemented
    return reflect_if_needed(
        unit_sprite[source_unit->type()].get_sprite(STANDING, source_unit->attack_progress()), side);
}

sf::Sprite sprite_supplier::get_cannon_sprite(cannon_type c_type, sprite_supplier::player_side side) {
    return reflect_if_needed(cannon_sprite[c_type], side);
}

sf::Sprite sprite_supplier::get_bullet_sprite(bullet_type b_type, sprite_supplier::player_side side) {
    return reflect_if_needed(bullet_sprite[b_type], side);
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

    for (auto &[a_type, sprite] : tower_front_sprite) {
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

void sprite_supplier::start_reading_Q_table() {
    std::thread load_Q_table([]() {
        std::unique_lock l(m);
        bot_actions_supplier::read_from_file();
        bot_actions_supplier::read_from_file();
        cond_var.notify_all();
    });
    load_Q_table.detach();
}

std::mutex &sprite_supplier::get_mutex() {
    return m;
}

std::condition_variable &sprite_supplier::get_cond_var() {
    return cond_var;
}
}  // namespace war_of_ages::client