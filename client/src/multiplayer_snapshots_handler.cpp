#include "../include/multiplayer_snapshots_handler.h"

namespace war_of_ages::client {
multiplayer_snapshots_handler &multiplayer_snapshots_handler::instance() {
    static multiplayer_snapshots_handler inst;
    return inst;
}

std::pair<player_snapshot, player_snapshot> multiplayer_snapshots_handler::get_snapshots() const {
    std::unique_lock l(m_mutex);
    return m_snapshots;
}

void multiplayer_snapshots_handler::set_snapshots(std::pair<player_snapshot, player_snapshot> new_snapshots) {
    std::unique_lock l(m_mutex);
    m_snapshots = std::move(new_snapshots);
}
}  // namespace war_of_ages::client