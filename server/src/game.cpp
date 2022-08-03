#include "game.h"
#include "database_handler.h"
#include "game_handler.h"
#include "server.h"
#include "server_bullet.h"
#include "server_cannon.h"
#include "server_unit.h"

namespace war_of_ages::server {
game::game(std::size_t id,
           std::string handle_p1,
           std::string handle_p2,
           std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
               game_post_action) noexcept
    : m_id(id),
      m_handle_p1(std::move(handle_p1)),
      m_handle_p2(std::move(handle_p2)),
      m_game_post_action(std::move(game_post_action)),
      m_state(server_unit_factory, server_bullet_factory, server_cannon_factory, m_handle_p1, m_handle_p2) {
    message<messages_type> msg_p1, msg_p2;
    msg_p1.header.id = msg_p2.header.id = messages_type::GAME_START;
    msg_p1.insert_container(m_handle_p2);
    msg_p2.insert_container(m_handle_p1);
    server::instance().send_message(m_handle_p1, msg_p1);
    server::instance().send_message(m_handle_p2, msg_p2);
}

bool game::apply_command(const std::string &handle, std::unique_ptr<game_command> command) {
    std::unique_lock l(m_mutex);
    assert(handle == m_handle_p1 || handle == m_handle_p2);
    bool result = false;
    if (handle == m_handle_p1) {
        result = m_state.apply_command(game_state::side::FIRST, std::move(command));
    } else {
        result = m_state.apply_command(game_state::side::SECOND, std::move(command));
    }
    if (m_state.get_game_status() != game_status::PROCESSING) {
        // std::cerr << "GAME [" << m_id << "]: GAME_STATE: NOT PROCESSING, ASSIGN RESULT & FINISH\n";
        m_result = m_state.get_game_status();
    } else {
        //        send_snapshots_lock_held();
    }
    return result;
}

void game::update() {
    // std::cout << "UPDATED(1)" << std::flush;
    std::unique_lock l(m_mutex);
    // std::cout << "UPDATED(2)" << std::flush;
    m_state.update({}, {});
    if (m_state.get_game_status() != game_status::PROCESSING) {
        // std::cerr << "GAME [" << m_id << "]: GAME_STATE: NOT PROCESSING, ASSIGN RESULT & FINISH\n";
        m_result = m_state.get_game_status();
    }
}

void game::user_gave_up(const std::string &handle) {
    // std::cerr << "GAME [" << m_id << "]: USER_GAVE_UP (" << handle << ")" << std::endl;
    std::unique_lock l(m_mutex);
    // std::cerr << "GAME [" << m_id << "]: USER_GAVE_UP (" << handle << ")" << std::endl;
    assert(handle == m_handle_p1 || handle == m_handle_p2);
    m_result = (m_handle_p1 == handle ? game_status::P2_WON : game_status::P1_WON);
    m_finish_reason = unusual_finish_reason::USER_GAVE_UP;
}

void game::user_disconnected(const std::string &handle) {
    // std::cerr << "GAME [" << m_id << "]: USER_DISCONNECTED (" << handle << ")" << std::endl;
    // IMPORTANT! DO NOT LOCK MUTEX
    std::unique_lock l(m_mutex);
    assert(handle == m_handle_p1 || handle == m_handle_p2);
    // TODO: if a little time has passed then declare a draw
    m_result = (m_handle_p1 == handle ? game_status::P2_WON : game_status::P1_WON);
    m_finish_reason = unusual_finish_reason::USER_DISCONNECTED;
}

bool game::is_finished() const {
    std::unique_lock l(m_mutex);
    return m_result != game_status::PROCESSING;
}

const std::string &game::get_handle_p1() const {
    std::unique_lock l(m_mutex);
    return m_handle_p1;
}

const std::string &game::get_handle_p2() const {
    std::unique_lock l(m_mutex);
    return m_handle_p2;
}

void game::fill_body_with_snapshot(message<messages_type> &msg, const player_snapshot &p_snapshot) noexcept {
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
}

void game::send_snapshots_lock_held() const {
    auto [snapshot_p1, snapshot_p2] = m_state.snapshot_players();
    message<messages_type> msg_p1, msg_p2;
    fill_body_with_snapshot(msg_p1, snapshot_p1), fill_body_with_snapshot(msg_p1, snapshot_p2);
    fill_body_with_snapshot(msg_p2, snapshot_p2), fill_body_with_snapshot(msg_p2, snapshot_p1);
    server::instance().send_message(m_handle_p1, msg_p1);
    server::instance().send_message(m_handle_p2, msg_p2);
}

std::size_t game::get_id() const {
    std::unique_lock l(m_mutex);
    return m_id;
}

std::pair<std::string, std::string> game::get_winner_loser_lock_held() {
    assert(m_result != game_status::PROCESSING);
    return (m_result == game_status::P1_WON ? std::pair{m_handle_p1, m_handle_p2}
                                            : std::pair{m_handle_p2, m_handle_p1});
}

void game::finish_game() {
    std::unique_lock l(m_mutex);

    // std::cerr << "GAME [" << m_id << "]: NOT PROCESSING, FINISH\n";
    // std::cerr << "  m_unusual_finish_reason == ";
    message<messages_type> msg_winner, msg_loser;
    msg_winner << static_cast<std::uint8_t>(1);
    msg_loser << static_cast<std::uint8_t>(0);
    if (!m_finish_reason.has_value()) {
        // std::cerr << " __NOT_INITED__" << std::endl;
        msg_winner.header.id = msg_loser.header.id = messages_type::GAME_FINISHED;
    } else if (m_finish_reason.value() == unusual_finish_reason::USER_GAVE_UP) {
        // std::cerr << " __USER_GAVE_UP__" << std::endl;
        msg_winner.header.id = msg_loser.header.id = messages_type::GAME_GIVE_UP;
    } else if (m_finish_reason.value() == unusual_finish_reason::USER_DISCONNECTED) {
        // std::cerr << " __USER_DISCONNECTED__" << std::endl;
        msg_winner.header.id = msg_loser.header.id = messages_type::GAME_DISCONNECTED;
    }
    auto [winner, loser] = get_winner_loser_lock_held();
    server::instance().send_message(winner, msg_winner);
    server::instance().send_message(loser, msg_loser);
    m_game_post_action(winner, loser);

    database_handler::get_instance().add_game(database_handler::Result::WIN, winner, loser);
}

void game::send_snapshots() {
    std::unique_lock l(m_mutex);
    send_snapshots_lock_held();
}

std::string game::get_enemy_handle(const std::string &handle) const {
    std::unique_lock l(m_mutex);
    return get_enemy_handle_lock_held(handle);
}

std::string game::get_enemy_handle_lock_held(const std::string &handle) const {
    return m_handle_p1 == handle ? m_handle_p2 : m_handle_p1;
}

}  // namespace war_of_ages::server