#include <cassert>
#include "../include/multiplayer_snapshots_handler.h"

namespace war_of_ages::client {
multiplayer_snapshots_handler &multiplayer_snapshots_handler::instance() {
    static multiplayer_snapshots_handler inst;
    return inst;
}

std::pair<player_snapshot, player_snapshot> multiplayer_snapshots_handler::get_snapshots() const {
    std::unique_lock l(m_mutex);
    assert(m_new_snapshot == true);
    m_new_snapshot = false;
    return m_snapshots;
}

void multiplayer_snapshots_handler::set_snapshots(std::pair<player_snapshot, player_snapshot> new_snapshots) {
    std::unique_lock l(m_mutex);
    m_snapshots = std::move(new_snapshots);
    m_new_snapshot = true;
}

bool multiplayer_snapshots_handler::new_snapshot_exists() const {
    std::unique_lock l(m_mutex);
    return m_new_snapshot;
}

void multiplayer_snapshots_handler::reset() {
    std::unique_lock l(m_mutex);
    m_new_snapshot = false;
}

}  // namespace war_of_ages::client