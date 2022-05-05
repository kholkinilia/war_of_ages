#ifndef WAR_OF_AGES_GAME_HANDLER_H
#define WAR_OF_AGES_GAME_HANDLER_H

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "../../common/game_logic/include/game_commands.h"
#include "game.h"

namespace war_of_ages {
struct game_handler {
    [[nodiscard]] static game_handler &instance() noexcept;
    ~game_handler() = default;
    game_handler(const game_handler &other) = delete;
    game_handler(game_handler &&other) = delete;
    game_handler &operator=(const game_handler &other) = delete;
    game_handler &operator=(game_handler &&other) = delete;

    void add_game(const std::string &first_handle, const std::string &second_handle);
    void apply_command(const std::string &handle, std::unique_ptr<game_command> command);
    void update_games();

private:
    game_handler() noexcept = default;

    std::unordered_map<std::string, std::size_t> m_game_by_handle;
    std::vector<game> m_games;
    std::mutex m_mutex;
};
}  // namespace war_of_ages

#endif  // WAR_OF_AGES_GAME_HANDLER_H
