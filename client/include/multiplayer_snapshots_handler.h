#include <mutex>
#include <utility>
#include "../../common/game_logic/include/player.h"

#ifndef WAR_OF_AGES_MULTIPLAYER_GAME_HANDLER_H
#define WAR_OF_AGES_MULTIPLAYER_GAME_HANDLER_H

namespace war_of_ages::client {
struct multiplayer_snapshots_handler {
    [[nodiscard]] static multiplayer_snapshots_handler &instance();
    ~multiplayer_snapshots_handler() = default;
    multiplayer_snapshots_handler(const multiplayer_snapshots_handler &other) = delete;
    multiplayer_snapshots_handler(multiplayer_snapshots_handler &&other) = delete;
    multiplayer_snapshots_handler &operator=(const multiplayer_snapshots_handler &other) = delete;
    multiplayer_snapshots_handler &operator=(multiplayer_snapshots_handler &&other) = delete;

    void set_snapshots(std::pair<player_snapshot, player_snapshot> new_snapshots);
    [[nodiscard]] std::pair<player_snapshot, player_snapshot> get_snapshots() const;

private:
    multiplayer_snapshots_handler() = default;
    std::pair<player_snapshot, player_snapshot> m_snapshots;
    mutable std::mutex m_mutex;
};
}

#endif  // WAR_OF_AGES_MULTIPLAYER_GAME_HANDLER_H
