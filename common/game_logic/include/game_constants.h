#ifndef WAR_OF_AGES_GAME_CONSTANTS_H
#define WAR_OF_AGES_GAME_CONSTANTS_H

#include "../../../client/include/game_object_size_constants.h"
namespace war_of_ages {

constexpr int NUM_OF_AGES = 2;
constexpr int UNITS_PER_AGE = 3;
constexpr int CANNONS_PER_AGE = 3;
constexpr int MAX_CANNON_SLOTS = 3;
constexpr int UNITS_QUEUE_SIZE = 5;

constexpr int NUM_OF_UNITS = NUM_OF_AGES * UNITS_PER_AGE;
constexpr int NUM_OF_CANNONS = NUM_OF_AGES * CANNONS_PER_AGE;

constexpr int INITIAL_MONEY = 200'000;

constexpr int CANNONS_SLOTS_COORD_X[MAX_CANNON_SLOTS] = {-100, -100, -100};
constexpr int CANNONS_SLOTS_COORD_Y[MAX_CANNON_SLOTS] = {
    200 + client::Y_COORD_DELTA, 300 + client::Y_COORD_DELTA, 400 + client::Y_COORD_DELTA};
constexpr int CANNONS_SLOTS_COSTS[MAX_CANNON_SLOTS] = {0, 500, 5000};  // IDK, these are random values

constexpr int NEXT_AGE_EXP[NUM_OF_AGES] = {400, 2000, /*14000, 40000, 200000*/};

constexpr float FIELD_HEIGHT_PXLS = 1080;  // IDK, random value
constexpr float FIELD_LENGTH_PXLS = client::ROAD_WIDTH - 2 * client::TOWER_WIDTH;

constexpr float ULT_COOLDOWN = 60.0;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_CONSTANTS_H
