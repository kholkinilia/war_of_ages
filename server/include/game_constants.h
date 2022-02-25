#ifndef WAR_OF_AGES_GAME_CONSTANTS_H
#define WAR_OF_AGES_GAME_CONSTANTS_H

namespace war_of_ages {

const int NUM_OF_AGES = 4;
const int UNITS_PER_AGE = 3;
const int CANNONS_PER_AGE = 3;
const int MAX_CANNON_SLOTS = 3;
const int UNITS_QUEUE_SIZE = 5;

const int NUM_OF_UNITS = NUM_OF_AGES * UNITS_PER_AGE;
const int NUM_OF_CANNONS = NUM_OF_AGES * CANNONS_PER_AGE;

const int INITIAL_MONEY = 200;

const int CANNONS_SLOTS_COORD_X[] = {0, 0, 0};
const int CANNONS_SLOTS_COORD_Y[] = {200, 300, 400};  // IDK, these are random values
const int CANNONS_SLOTS_COSTS[] = {0, 500, 5000};     // IDK, these are random values

const int FIELD_LENGTH_PXLS = 2300;

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_CONSTANTS_H
