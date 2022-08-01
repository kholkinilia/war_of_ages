#include "multiplayer_snapshots_handler.h"
#include <cassert>

namespace war_of_ages::client {
multiplayer_snapshots_handler &multiplayer_snapshots_handler::instance() {
    static multiplayer_snapshots_handler inst;
    return inst;
}

std::pair<player_snapshot, player_snapshot> multiplayer_snapshots_handler::get_snapshots() const {
    std::unique_lock l(m_mutex);
    assert(m_snapshot_initialized == true);
    return m_cur_game->snapshot_players();
}

void multiplayer_snapshots_handler::set_snapshots(std::pair<player_snapshot, player_snapshot> new_snapshots) {
    std::unique_lock l(m_mutex);
    m_cur_game->set_state(new_snapshots.first, new_snapshots.second);
    m_snapshots = std::move(new_snapshots);
    m_snapshot_initialized = true;
}

bool multiplayer_snapshots_handler::is_snapshot_initialized() const {
    std::unique_lock l(m_mutex);
    return m_snapshot_initialized;
}

void multiplayer_snapshots_handler::reset() {
    std::unique_lock l(m_mutex);
    m_snapshot_initialized = false;
    m_cur_game.reset();
}

void multiplayer_snapshots_handler::start_game() {
    std::unique_lock l(m_mutex);
    m_cur_game = std::make_unique<game_state>();
    m_snapshots = m_cur_game->snapshot_players();
    m_snapshot_initialized = true;
}

void multiplayer_snapshots_handler::update_game() {
    std::unique_lock l(m_mutex);
    assert(m_cur_game != nullptr);
    m_cur_game->update({}, {});
}

}  // namespace war_of_ages::client