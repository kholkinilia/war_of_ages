#ifndef WAR_OF_AGES_GAME_H
#define WAR_OF_AGES_GAME_H

#include <cstdint>
#include <functional>
#include <string>
#include "../../common/network/include/message.h"
#include "game_state.h"
#include "messages_type.h"

namespace war_of_ages::server {

struct game {
    enum class result {
        P1_WON,
        P2_WON,
        PROCESSING,
    };

    game(std::size_t id,
         std::string handle_p1,
         std::string handle_p2,
         std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
             game_post_action) noexcept;

    void apply_command(const std::string &handle, std::unique_ptr<game_command> command) noexcept;
    void update();
    void user_gave_up(const std::string &handle);
    void user_disconnected(const std::string &handle);
    void call_post_game_actions();

    void set_result(result game_result);
    [[nodiscard]] std::size_t get_id() const noexcept;
    [[nodiscard]] bool is_finished() const noexcept;
    [[nodiscard]] const std::string &get_handle_p1() const noexcept;
    [[nodiscard]] const std::string &get_handle_p2() const noexcept;

private:
    static void fill_body_with_snapshot(message<messages_type> &msg,
                                        const player_snapshot &p_snapshot) noexcept;
    void send_snapshots() const;
    [[nodiscard]] std::pair<std::string, std::string> get_winner_loser();

    std::size_t m_id;
    std::string m_handle_p1;
    std::string m_handle_p2;
    result m_result = result::PROCESSING;
    game_state m_state;
    std::function<void(const std::string &handle_winner, const std::string &handle_loser)> m_game_post_action;
};
}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_GAME_H
