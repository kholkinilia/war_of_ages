#include "../include/game_state.h"
#include <mutex>
#include <vector>
#include "../include/player.h"

namespace war_of_ages {

void game_state::update(std::vector<command> p1_commands, std::vector<command> p2_commands, int dt) {
    // p1.update(p2.units(), dt); TODO: uncomment when player{.h, .cpp} is updated
    // p2.update(p1.units(), dt);

    // TODO: implement new commands handling

    p1.clear_dead_objects();
    p2.clear_dead_objects();
}

}  // namespace war_of_ages