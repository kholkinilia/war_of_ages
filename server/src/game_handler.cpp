#include "game_handler.h"
#include <thread>

namespace war_of_ages::server {
game_handler &game_handler::instance() noexcept {
    static game_handler inst;
    return inst;
}

void game_handler::add_game(
    std::string handle_p1,
    std::string handle_p2,
    std::function<void(const std::string &handle_winner, const std::string &handle_loser)> game_post_action) {
    std::unique_lock l(m_mutex);
    assert(!user_exists_lock_held(handle_p1) && !user_exists_lock_held(handle_p2));
    m_game_id_by_handle.insert({handle_p1, m_free_id});
    m_game_id_by_handle.insert({handle_p2, m_free_id});
    m_games[m_free_id] = std::make_shared<game>(m_free_id, std::move(handle_p1), std::move(handle_p2),
                                                std::move(game_post_action));

    std::thread([cur_game = m_games[m_free_id]]() {
        float prev_time = 0;
        while (!cur_game->is_finished()) {
            auto cur_time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
            if (cur_time - prev_time >= 0.003) {
                prev_time = cur_time;
                cur_game->update();
            }
        }
        game_handler::instance().remove_game(cur_game->get_id());
        cur_game->finish_game();
    }).detach();

    m_free_id++;
}

void game_handler::apply_command(const std::string &handle, std::unique_ptr<game_command> command) {
    std::unique_lock l(m_mutex);
    if (!user_exists_lock_held(handle)) {
        return;
    }
    m_games[m_game_id_by_handle.at(handle)]->apply_command(handle, std::move(command));
}

void game_handler::user_gave_up(const std::string &handle) {
    std::unique_lock l(m_mutex);
    if (!user_exists_lock_held(handle)) {
        return;
    }
    m_games[m_game_id_by_handle.at(handle)]->user_gave_up(handle);
}

void game_handler::user_disconnected(const std::string &handle) {
    std::unique_lock l(m_mutex);
    if (!user_exists_lock_held(handle)) {
        return;
    }
    m_games[m_game_id_by_handle.at(handle)]->user_disconnected(handle);
}

bool game_handler::user_exists_lock_held(const std::string &handle) const noexcept {
    return m_game_id_by_handle.find(handle) != m_game_id_by_handle.end();
}

void game_handler::remove_game(std::size_t game_id) {
    std::unique_lock l(m_mutex);
    auto &game_ = m_games[game_id];
    m_game_id_by_handle.erase(game_->get_handle_p1());
    m_game_id_by_handle.erase(game_->get_handle_p2());
    m_games.erase(game_id);
}
}  // namespace war_of_ages::server