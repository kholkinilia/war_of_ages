#ifndef WAR_OF_AGES_GAME_STATE_H
#define WAR_OF_AGES_GAME_STATE_H

#include <memory>
#include <vector>
#include "game_commands.h"
#include "player.h"
#include "woa_time.h"

namespace war_of_ages {

enum class game_status { P1_WON, PROCESSING, P2_WON };

struct game_state {
private:
    player p1, p2;
    std::uint64_t m_state_time;
    std::uint64_t m_start_time;

public:
    enum class side { FIRST, SECOND };

    explicit game_state(
        const std::function<std::shared_ptr<unit>(unit_type)> &unit_factory,
        const std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)>
            &bullet_factory);
    explicit game_state(
        std::uint64_t start_time,
        const std::function<std::shared_ptr<unit>(unit_type)> &unit_factory,
        const std::function<std::shared_ptr<bullet>(bullet_type, const vec2f &, const vec2f &)>
            &bullet_factory);

    void update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                const std::vector<std::unique_ptr<game_command>> &p2_commands);
    bool apply_command(side player_side, std::unique_ptr<game_command> cmd);

    [[nodiscard]] std::pair<player_snapshot, player_snapshot> snapshot_players() const;
    [[nodiscard]] game_status get_game_status() const;
    void set_state(const player_snapshot &p1_snap, const player_snapshot &p2_snap);
    void return_from_pause() noexcept;

    [[nodiscard]] std::uint64_t get_start_time() const noexcept;
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_STATE_H
