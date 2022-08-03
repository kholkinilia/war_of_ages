#ifndef WAR_OF_AGES_SPRITE_SUPPLIER_H
#define WAR_OF_AGES_SPRITE_SUPPLIER_H

#include <SFML/Graphics.hpp>
#include <condition_variable>
#include <fstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "animation_supplier.h"
#include "age.h"
#include "bullet.h"
#include "cannon.h"
#include "unit.h"

namespace war_of_ages::client {

struct sprite_supplier {
public:
    enum class player_side { LEFT, RIGHT };

    sf::Sprite get_background_sprite(age_type a_type);
    sf::Sprite get_road_sprite(age_type a_type);
    sf::Sprite get_tower_sprite(age_type a_type, std::size_t number_of_slots, player_side side);
    sf::Sprite get_tower_front_sprite(age_type a_type, std::size_t number_of_slots, player_side side);
    sf::Sprite get_cannon_slot_sprite(std::pair<age_type, int> cs_type, player_side side);
    sf::Sprite get_unit_sprite(const std::shared_ptr<unit>& source_unit, player_side side);
    sf::Sprite get_cannon_sprite(cannon_type c_type, player_side side);
    sf::Sprite get_bullet_sprite(bullet_type b_type, player_side side);

    sprite_supplier(const sprite_supplier &) = delete;
    sprite_supplier(sprite_supplier &&) = default;
    sprite_supplier &operator=(const sprite_supplier &) = delete;
    sprite_supplier &operator=(sprite_supplier &&) = default;

    ~sprite_supplier();

    [[nodiscard]] static sprite_supplier &get_instance();

    static void start_reading_Q_table();
    [[nodiscard]] static std::mutex &get_mutex();
    [[nodiscard]] static std::condition_variable &get_cond_var();

private:
    std::unordered_map<age_type, sf::Sprite> background_sprite;
    std::unordered_map<age_type, sf::Sprite> road_sprite;

    std::map<std::pair<age_type, int>, sf::Sprite> tower_sprite;
    std::map<std::pair<age_type, int>, sf::Sprite> tower_front_sprite;
    std::map<std::pair<age_type, int>, sf::Sprite> cannon_slots_sprite;

    std::unordered_map<unit_type, animation_supplier> unit_sprite;
    std::unordered_map<cannon_type, sf::Sprite> cannon_sprite;
    std::unordered_map<bullet_type, sf::Sprite> bullet_sprite;

    sprite_supplier();

    static sf::Sprite create_sprite_instance(const std::string &filename, int width, int height);
    static sf::Sprite reflect_if_needed(sf::Sprite sprite, player_side side);

    static inline std::condition_variable cond_var;
    static inline std::mutex m;
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_SPRITE_SUPPLIER_H