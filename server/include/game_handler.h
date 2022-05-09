#ifndef WAR_OF_AGES_GAME_HANDLER_H
#define WAR_OF_AGES_GAME_HANDLER_H

#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../common/game_logic/include/game_commands.h"
#include "game.h"

namespace war_of_ages::server {
struct game_handler {
    [[nodiscard]] static game_handler &instance() noexcept;
    ~game_handler() = default;
    game_handler(const game_handler &other) = delete;
    game_handler(game_handler &&other) = delete;
    game_handler &operator=(const game_handler &other) = delete;
    game_handler &operator=(game_handler &&other) = delete;

    void add_game(std::string handle_p1,
                  std::string handle_p2,
                  std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
                      game_post_action);
    void apply_command(const std::string &handle, std::unique_ptr<game_command> command);
    void update_games();

    void user_gave_up(const std::string &handle);
    void user_disconnected(const std::string &handle);

private:
    game_handler() noexcept = default;

    [[nodiscard]] bool user_exists_lock_held(const std::string &handle) const noexcept;
    [[nodiscard]] bool user_exists(const std::string &handle) const;
    void remove_game(std::size_t game_index) noexcept;

    std::unordered_map<std::string, std::size_t> m_game_by_handle;
    std::vector<game> m_games;
    mutable std::mutex m_mutex;
};
}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_GAME_HANDLER_H
