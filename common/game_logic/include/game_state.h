#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include <memory>
#include <vector>
#include "woa_time.h"
#include "game_commands.h"
#include "player.h"

namespace war_of_ages {

enum class game_status { P1_WON, PROCESSING, P2_WON };

struct game_state {
private:
    player p1, p2;
    std::uint64_t m_state_time;
    std::uint64_t m_start_time;

public:
    game_state();
    explicit game_state(std::uint64_t start_time);

    void update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                const std::vector<std::unique_ptr<game_command>> &p2_commands);
    [[nodiscard]] std::pair<player_snapshot, player_snapshot> snapshot_players() const;
    [[nodiscard]] game_status get_game_status() const;
    void set_state(const player_snapshot &p1_snap, const player_snapshot &p2_snap);
    void return_from_pause() noexcept;

    [[nodiscard]] std::uint64_t get_start_time() const noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
