#include "../include/game.h"
#include "../include/server.h"
#include <ctime>

// FIXME: get rid of '0' in m_state.update(). In a good way game_state should calculate time by itself

namespace war_of_ages {
game::game(std::string handle_p1, std::string handle_p2) noexcept
    : m_handle_p1(std::move(handle_p1)),
      m_handle_p2(std::move(handle_p2)),
      m_state(1.f * clock() / CLOCKS_PER_SEC) {
    // TODO: send GAME_START message
}

void game::apply_command(const std::string &handle, std::unique_ptr<game_command> command) noexcept {
    std::vector<std::unique_ptr<game_command>> current_player_actions;
    current_player_actions.push_back(std::move(command));
    if (handle == m_handle_p1) {
        m_state.update(current_player_actions, {}, 0);
    } else if (handle == m_handle_p2) {
        m_state.update({}, current_player_actions, 0);
    } else {
        assert(false);
    }
}

void game::update() {
    m_state.update({}, {}, 0);
    if (is_finished()) {
        // TODO: send GAME_FINISHED message
    } else {
        send_snapshots();
    }
}

void game::user_gave_up(const std::string &handle) {
    // TODO: implement
}

bool game::is_finished() const noexcept {
    return m_state.get_game_status() != game_status::PROCESSING;
}

const std::string &game::get_handle_p1() const noexcept {
    return m_handle_p1;
}

const std::string &game::get_handle_p2() const noexcept {
    return m_handle_p2;
}

message<messages_type> game::get_msg_snapshot(const player_snapshot &p_snapshot) noexcept {
    message<messages_type> result_msg;
    // TODO: create a GAME_STATE message
    return result_msg;
}

void game::send_snapshots() const {
    auto [snapshot_p1, snapshot_p2] = m_state.snapshot_players();
    message<messages_type> msg_p1 = get_msg_snapshot(snapshot_p1);
    message<messages_type> msg_p2 = get_msg_snapshot(snapshot_p2);
    server::instance().send_message(m_handle_p1, msg_p1);
    server::instance().send_message(m_handle_p2, msg_p2);
}
}  // namespace war_of_ages