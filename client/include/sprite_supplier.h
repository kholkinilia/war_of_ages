#ifndef WAR_OF_AGES_SPRITE_SUPPLIER_H
#define WAR_OF_AGES_SPRITE_SUPPLIER_H

#include "../../game_logic/include/age.h"
#include "../../game_logic/include/unit.h"
#include "../../game_logic/include/cannon.h"
#include "../../game_logic/include/bullet.h"
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>

namespace war_of_ages {

enum class direction {
    LEFT,
    RIGHT
};

struct sprite_supplier {
private:
    inline const static std::map<age_type, std::string> tower_texture_file {
        {age_type::STONE, "../resources/game/towers/stone/tower.png"}
    };
    inline const static std::map<unit_type, std::string> unit_texture_file {
        {unit_type::PEASANT, "../resources/game/towers/stone/peasant.png"},
        {unit_type::ARCHER, "../resources/game/towers/stone/archer.png"},
        {unit_type::CHARIOT, "../resources/game/towers/stone/chariot.png"}
    };
    inline const static std::map<cannon_type, std::string> cannon_texture_file {
        {cannon_type::STONE_LEVEL_1, "../resources/game/cannons/stone/level_1.png"},
        {cannon_type::STONE_LEVEL_2, "../resources/game/cannons/stone/level_2.png"},
        {cannon_type::STONE_LEVEL_3, "../resources/game/cannons/stone/level_3.png"},
    };
    inline const static std::map<int, std::string> cannon_slots_texture_file {
        {1, "../resources/game/cannon_slots/stone/level_1.png"},
        {2, "../resources/game/cannon_slots/stone/level_2.png"},
        {3, "../resources/game/cannon_slots/stone/level_3.png"},
    };
    inline const static std::map<bullet_type, std::string> bullet_texture_file {
        {bullet_type::STONE_LEVEL_1, "../resources/game/bullets/stone/level_1.png"},
        {bullet_type::STONE_LEVEL_2, "../resources/game/bullets/stone/level_2.png"},
        {bullet_type::STONE_LEVEL_3, "../resources/game/bullets/stone/level_3.png"},
        {bullet_type::STONE_ULT, "../resources/game/bullets/stone/ult.png"},
    };

public:

};

}

#endif  // WAR_OF_AGES_SPRITE_SUPPLIER_H
