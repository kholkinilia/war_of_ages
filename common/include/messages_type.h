#ifndef WAR_OF_AGES_MESSAGES_TYPE_H
#define WAR_OF_AGES_MESSAGES_TYPE_H

#include <cstdint>
#include <unordered_map>

namespace war_of_ages {
enum class messages_type {
    AUTH_LOGIN,
    AUTH_LOGOUT,

    GAME_BUY_UNIT,
    GAME_BUY_CANNON,
    GAME_BUY_CANNON_SLOT,
    GAME_SELL_CANNON,
    GAME_USE_ULT,
    GAME_UPGRADE_AGE,
    GAME_GIVE_UP,
    GAME_DISCONNECTED,
    GAME_START,
    GAME_FINISHED,
    GAME_STATE,

    RANDOMGAME_JOIN,
    RANDOMGAME_LEAVE,

    ROOM_JOIN,
    ROOM_LEAVE,
    ROOM_CHANGE_STATUS,

    TOURNAMENT_CREATE,
    TOURNAMENT_JOIN,
    TOURNAMENT_LEAVE,
    TOURNAMENT_STATE,
    TOURNAMENT_ADD_PLAYER,
    TOURNAMENT_REMOVE_PLAYER,
    TOURNAMENT_ADD_RESULT,

    SERVER_IGNORE,
    SERVER_PING,
};

const std::unordered_map<messages_type, std::uint32_t> valid_body_size = {
    {messages_type::AUTH_LOGIN, -1},
    {messages_type::AUTH_LOGOUT, -1},

    {messages_type::GAME_BUY_UNIT, 1},
    {messages_type::GAME_BUY_CANNON, 2},
    {messages_type::GAME_BUY_CANNON_SLOT, 0},
    {messages_type::GAME_SELL_CANNON, 1},
    {messages_type::GAME_USE_ULT, 0},
    {messages_type::GAME_UPGRADE_AGE, 0},
    {messages_type::GAME_GIVE_UP, 0},

    {messages_type::GAME_START, -1},
    {messages_type::GAME_FINISHED, -1},
    {messages_type::GAME_STATE, -1},

    {messages_type::RANDOMGAME_JOIN, -1},
    {messages_type::RANDOMGAME_LEAVE, -1},

    {messages_type::ROOM_JOIN, -1},
    {messages_type::ROOM_LEAVE, 0},
    {messages_type::ROOM_CHANGE_STATUS, 0},

    // TODO: fill by Ilya
    {messages_type::TOURNAMENT_CREATE, -1},
    {messages_type::TOURNAMENT_JOIN, -1},
    {messages_type::TOURNAMENT_LEAVE, -1},
    {messages_type::TOURNAMENT_STATE, -1},
    {messages_type::TOURNAMENT_ADD_PLAYER, -1},
    {messages_type::TOURNAMENT_REMOVE_PLAYER, -1},
    {messages_type::TOURNAMENT_ADD_RESULT, -1},

    {messages_type::SERVER_IGNORE, 0},
    {messages_type::SERVER_PING, 0}};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MESSAGES_TYPE_H
