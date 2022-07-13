#include "game_state.h"
#include <ctime>
#include <memory>
#include <vector>

namespace war_of_ages {

void game_state::update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                        const std::vector<std::unique_ptr<game_command>> &p2_commands) {
    float time = static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
    p1.update(p2, time - state_time);
    p2.update(p1, time - state_time);
    state_time = time;

    for (auto &command : p1_commands) {
        command->apply(p1);
    }
    for (auto &command : p2_commands) {
        command->apply(p2);
    }

    p1.clear_dead_objects();
    p2.clear_dead_objects();
}

std::pair<player_snapshot, player_snapshot> game_state::snapshot_players() const {
    return {p1.snapshot(), p2.snapshot()};
}

game_state::game_state() : state_time(static_cast<float>(std::clock()) / CLOCKS_PER_SEC) {
}

game_status game_state::get_game_status() const {
    if (!p1.is_alive()) {
        return game_status::P2_WON;
    }
    if (!p2.is_alive()) {
        return game_status::P1_WON;
    }
    return game_status::PROCESSING;
}

void game_state::return_from_pause() noexcept {
    float return_time = static_cast<float>(std::clock()) / CLOCKS_PER_SEC;
    state_time = return_time;
}

}  // namespace war_of_ages
