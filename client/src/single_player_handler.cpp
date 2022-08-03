#include "single_player_handler.h"
#include "bot_actions_supplier.h"
#include "player_actions_supplier.h"
#include "client_unit.h"

namespace war_of_ages::client {

single_player_handler &single_player_handler::instance() {
    static single_player_handler handler;
    return handler;
}

void single_player_handler::start_game() {
    m_game_state = std::make_unique<game_state>(client_unit_factory);
}

void single_player_handler::update_game() {
    auto [p1, p2] = m_game_state->snapshot_players();
    m_game_state->update(m_player_type == player_type::BOT
                             ? bot_actions_supplier::instance().get_actions({p1, p2})
                             : player_actions_supplier::instance().get_actions(),
                         bot_actions_supplier::instance().get_actions({p2, p1}));
}

void single_player_handler::finish_game() {
    m_game_state = nullptr;
    player_actions_supplier::instance().clear_actions();
}

void single_player_handler::return_from_pause() {
    m_game_state->return_from_pause();
}

void single_player_handler::change_player_type(player_type new_type) {
    m_player_type = new_type;
}

game_status single_player_handler::get_game_status() const noexcept {
    return m_game_state->get_game_status();
}

single_player_handler::player_type single_player_handler::get_type() const noexcept {
    return m_player_type;
}

std::pair<player_snapshot, player_snapshot> single_player_handler::get_snapshot() const noexcept {
    return m_game_state->snapshot_players();
}

}  // namespace war_of_ages::client