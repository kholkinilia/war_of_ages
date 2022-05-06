#include "../include/game_handler.h"

namespace war_of_ages {
game_handler &game_handler::instance() noexcept {
    static game_handler inst;
    return inst;
}

void game_handler::add_game(std::string handle_p1, std::string handle_p2) {
    assert(!user_exists(handle_p1) && !user_exists(handle_p2));
    std::unique_lock l(m_mutex);
    m_game_by_handle.insert({handle_p1, m_games.size()});
    m_game_by_handle.insert({handle_p2, m_games.size()});
    // FIXME
    // m_games.push_back(game(std::move(handle_p1), std::move(handle_p2));
}

void game_handler::apply_command(const std::string &handle, std::unique_ptr<game_command> command) {
    if (!user_exists(handle)) {
        return;
    }
    std::unique_lock l(m_mutex);
    m_games[m_game_by_handle.at(handle)].apply_command(handle, std::move(command));
}

void game_handler::update_games(server &server_ref) {
    std::unique_lock l(m_mutex);
    for (std::size_t game_index = 0; game_index < m_games.size(); ++game_index) {
        auto &game_ = m_games[game_index];
        game_.update(server_ref);
        if (game_.is_finished()) {
            remove_game(game_index);
        }
    }
}

void game_handler::user_gave_up(const std::string &handle, server &server_ref) {
    if (!user_exists(handle)) {
        return;
    }
    std::size_t game_index = m_game_by_handle.at(handle);
    m_games[game_index].user_gave_up(handle, server_ref);
    remove_game(game_index);
}

bool game_handler::user_exists(const std::string &handle) const noexcept {
    std::unique_lock l(m_mutex);
    return m_game_by_handle.find(handle) != m_game_by_handle.end();
}

void game_handler::remove_game(std::size_t game_index) noexcept {
    auto &game_ = m_games[game_index];
    m_game_by_handle.erase(game_.get_handle_p1());
    m_game_by_handle.erase(game_.get_handle_p2());
    m_game_by_handle[m_games.back().get_handle_p1()] = game_index;
    m_game_by_handle[m_games.back().get_handle_p2()] = game_index;
    // FIXME
    // std::swap(m_games[game_index], m_games.back());
    m_games.pop_back();
}

}  // namespace war_of_ages