#include "../include/game_state.h"
#include <vector>
#include "../include/game_commands.h"
#include "../include/player.h"
#include <memory>

namespace war_of_ages {

void game_state::update(const std::vector<std::unique_ptr<game_command>> &p1_commands,
                        const std::vector<std::unique_ptr<game_command>> &p2_commands,
                        double dt) {
    p1.update(p2, dt);
    p2.update(p1, dt);

    for (auto &command: p1_commands) {
        command->apply(p1);
    }
    for (auto &command: p2_commands) {
        command->apply(p2);
    }

    p1.clear_dead_objects();
    p2.clear_dead_objects();
}

std::pair<player_snapshot, player_snapshot> game_state::snapshot_players() {
    return {p1.snapshot(), p2.snapshot()};
}

}  // namespace war_of_ages
