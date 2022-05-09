#include "../include/game.h"
#include "../include/server.h"

namespace war_of_ages::server {
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
    //    std::cout << "updating game\n";
    if (is_finished()) {
        //        std::cout << "game is finidhed\n";
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
        //        std::cout << "sending snapshot\n";
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
    msg.insert_container(p_snapshot.units);
    msg.insert_container(p_snapshot.bullets);
    msg.insert_container(p_snapshot.cannons);
    msg.insert_container(p_snapshot.units_to_train);
    msg << p_snapshot.age;
    msg << p_snapshot.exp;
    msg << p_snapshot.money;
    msg << p_snapshot.m_ult_cooldown;
    msg << p_snapshot.m_training_time_left;
    return msg;
}

void game::send_snapshots() const {
    auto [snapshot_p1, snapshot_p2] = m_state.snapshot_players();
    message<messages_type> p1, p2;
    p1.header.id = p2.header.id = messages_type::GAME_STATE;

    p1.insert_container(snapshot_p1.units);
    p1.insert_container(snapshot_p1.bullets);
    p1.insert_container(snapshot_p1.cannons);
    p1.insert_container(snapshot_p1.units_to_train);
    p1 << snapshot_p1.age;
    p1 << snapshot_p1.exp;
    p1 << snapshot_p1.money;
    p1 << snapshot_p1.m_ult_cooldown;
    p1 << snapshot_p1.m_training_time_left;
    p1.insert_container(snapshot_p2.units);
    p1.insert_container(snapshot_p2.bullets);
    p1.insert_container(snapshot_p2.cannons);
    p1.insert_container(snapshot_p2.units_to_train);
    p1 << snapshot_p2.age;
    p1 << snapshot_p2.exp;
    p1 << snapshot_p2.money;
    p1 << snapshot_p2.m_ult_cooldown;
    p1 << snapshot_p2.m_training_time_left;

    p2.insert_container(snapshot_p2.units);
    p2.insert_container(snapshot_p2.bullets);
    p2.insert_container(snapshot_p2.cannons);
    p2.insert_container(snapshot_p2.units_to_train);
    p2 << snapshot_p2.age;
    p2 << snapshot_p2.exp;
    p2 << snapshot_p2.money;
    p2 << snapshot_p2.m_ult_cooldown;
    p2 << snapshot_p2.m_training_time_left;
    p2.insert_container(snapshot_p1.units);
    p2.insert_container(snapshot_p1.bullets);
    p2.insert_container(snapshot_p1.cannons);
    p2.insert_container(snapshot_p1.units_to_train);
    p2 << snapshot_p1.age;
    p2 << snapshot_p1.exp;
    p2 << snapshot_p1.money;
    p2 << snapshot_p1.m_ult_cooldown;
    p2 << snapshot_p1.m_training_time_left;

    server::instance().send_message(m_handle_p1, p1);
    server::instance().send_message(m_handle_p2, p2);
}
}  // namespace war_of_ages::server