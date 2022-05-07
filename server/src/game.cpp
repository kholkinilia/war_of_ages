#include "../include/game.h"
#include "../include/server.h"

namespace war_of_ages {
game::game(std::string handle_p1,
           std::string handle_p2,
           std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
               game_post_action) noexcept
    : m_handle_p1(std::move(handle_p1)),
      m_handle_p2(std::move(handle_p2)),
      m_game_post_action(std::move(game_post_action)) {
    message<messages_type> msg_p1, msg_p2;
    msg_p1.header.id = msg_p2.header.id = messages_type::GAME_START;
    msg_p1.insert_container(m_handle_p2);
    msg_p2.insert_container(m_handle_p1);
    server::instance().send_message(m_handle_p1, msg_p1);
    server::instance().send_message(m_handle_p2, msg_p2);
}

void game::apply_command(const std::string &handle, std::unique_ptr<game_command> command) noexcept {
    assert(handle == m_handle_p1 || handle == m_handle_p2);
    std::vector<std::unique_ptr<game_command>> current_player_actions;
    current_player_actions.push_back(std::move(command));
    if (handle == m_handle_p1) {
        m_state.update(current_player_actions, {});
    } else {
        m_state.update({}, current_player_actions);
    }
}

void game::update() {
    m_state.update({}, {});
    if (is_finished()) {
        message<messages_type> msg_p1, msg_p2;
        msg_p1.header.id = msg_p2.header.id = messages_type::GAME_FINISHED;
        if (m_state.get_game_status() == game_status::P1_WON) {
            msg_p1 << static_cast<std::uint8_t>(1);
            msg_p2 << static_cast<std::uint8_t>(0);
            m_game_post_action(m_handle_p1, m_handle_p2);
        } else {
            msg_p1 << static_cast<std::uint8_t>(0);
            msg_p2 << static_cast<std::uint8_t>(1);
            m_game_post_action(m_handle_p2, m_handle_p1);
        }
        server::instance().send_message(m_handle_p1, msg_p1);
        server::instance().send_message(m_handle_p2, msg_p2);
    } else {
        send_snapshots();
    }
}

void game::user_gave_up(const std::string &handle) {
    assert(handle == m_handle_p1 || handle == m_handle_p2);
    message<messages_type> msg_winner, msg_loser;
    msg_winner.header.id = msg_loser.header.id = messages_type::GAME_GIVE_UP;
    msg_winner << static_cast<std::uint8_t>(1);
    msg_loser << static_cast<std::uint8_t>(0);
    if (handle == m_handle_p2) {
        m_game_post_action(m_handle_p1, m_handle_p2);
        server::instance().send_message(m_handle_p1, msg_winner);
        server::instance().send_message(m_handle_p2, msg_loser);
    } else {
        m_game_post_action(m_handle_p2, m_handle_p1);
        server::instance().send_message(m_handle_p2, msg_winner);
        server::instance().send_message(m_handle_p1, msg_loser);
    }
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
    message<messages_type> msg;
    msg.header.id = messages_type::GAME_STATE;
    msg << p_snapshot.units << p_snapshot.bullets << p_snapshot.cannons << p_snapshot.units_to_train
        << p_snapshot.age << p_snapshot.exp << p_snapshot.money << p_snapshot.m_ult_cooldown
        << p_snapshot.m_training_time_left;
    return msg;
}

void game::send_snapshots() const {
    auto [snapshot_p1, snapshot_p2] = m_state.snapshot_players();
    message<messages_type> msg_p1 = get_msg_snapshot(snapshot_p1);
    message<messages_type> msg_p2 = get_msg_snapshot(snapshot_p2);
    server::instance().send_message(m_handle_p1, msg_p1);
    server::instance().send_message(m_handle_p2, msg_p2);
}
}  // namespace war_of_ages