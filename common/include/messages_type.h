#ifndef WAR_OF_AGES_MESSAGES_TYPE_H
#define WAR_OF_AGES_MESSAGES_TYPE_H

#include <cstdint>
#include <unordered_map>

namespace war_of_ages {
enum class messages_type {
    AUTH_LOGIN,
    AUTH_LOGIN_FAILED,
    AUTH_LOGIN_SUCCEEDED,
    AUTH_LOGOUT,
    AUTH_REGISTER,
    AUTH_REGISTER_FAILED,
    AUTH_REGISTER_SUCCEEDED,
    AUTH_ALREADY_USING,

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

    // Client -> Server
    ROOM_JOIN,
    ROOM_LEAVE,
    ROOM_SWITCH_STATUS,
    // Server -> Client
    ROOM_JOIN_RESPONSE,
    ROOM_ENEMY_JOINED,
    ROOM_LEAVE_SUCCESS,
    ROOM_ENEMY_LEAVED,
    ROOM_ENEMY_SWITCHED_STATUS,

    TOURNAMENT_CREATE,
    TOURNAMENT_JOIN,
    TOURNAMENT_LEAVE,
    TOURNAMENT_STATE,
    TOURNAMENT_ADD_PLAYER,
    TOURNAMENT_REMOVE_PLAYER,
    TOURNAMENT_ADD_RESULT,
    TOURNAMENT_ADD_MATCH,

    SERVER_IGNORE,
    SERVER_PING,

    CHAT_NEW_MESSAGE,
};

const std::unordered_map<messages_type, std::uint32_t> valid_body_size = {
    {messages_type::AUTH_LOGIN, -1},
    {messages_type::AUTH_LOGIN_FAILED, -1},
    {messages_type::AUTH_LOGIN_SUCCEEDED, -1},
    {messages_type::AUTH_LOGOUT, 0},
    {messages_type::AUTH_REGISTER, -1},
    {messages_type::AUTH_REGISTER_FAILED, -1},
    {messages_type::AUTH_REGISTER_SUCCEEDED, -1},
    {messages_type::AUTH_ALREADY_USING, 0},

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

    {messages_type::RANDOMGAME_JOIN, 0},
    {messages_type::RANDOMGAME_LEAVE, 0},

    {messages_type::ROOM_JOIN, -1},
    {messages_type::ROOM_LEAVE, 0},
    {messages_type::ROOM_SWITCH_STATUS, 0},

    {messages_type::TOURNAMENT_CREATE, -1},
    {messages_type::TOURNAMENT_JOIN, -1},
    {messages_type::TOURNAMENT_LEAVE, -1},
    {messages_type::TOURNAMENT_STATE, -1},
    {messages_type::TOURNAMENT_ADD_PLAYER, -1},
    {messages_type::TOURNAMENT_REMOVE_PLAYER, -1},
    {messages_type::TOURNAMENT_ADD_RESULT, -1},

    {messages_type::SERVER_IGNORE, 0},
    {messages_type::SERVER_PING, 0},

    {messages_type::CHAT_NEW_MESSAGE, -1}};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_MESSAGES_TYPE_H
