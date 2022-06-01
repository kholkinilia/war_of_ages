#ifndef WAR_OF_AGES_ROOM_HANDLER_H
#define WAR_OF_AGES_ROOM_HANDLER_H

#include <optional>
#include <string>

namespace war_of_ages::client {
struct room_handler {
    enum class player_status {
        READY,
        NOT_READY,
    };

    struct player_info {
        std::string handle;
        player_status status;
        int rate;
    };

    static room_handler &instance();

    void update_enemy(std::optional<player_info> enemy);

    /// call after the battle (to update rate) or after relogin
    void update_me(player_info me);

    void change_enemy_status(player_status status);
    void change_my_status(player_status status);
    void switch_enemy_status();
    void switch_my_status();

private:
    room_handler() noexcept = default;
    std::optional<player_info> players[2];
};
}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_ROOM_HANDLER_H
