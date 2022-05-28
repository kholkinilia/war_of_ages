#ifndef WAR_OF_AGES_GAME_H
#define WAR_OF_AGES_GAME_H

#include <cstdint>
#include <functional>
#include <optional>
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

    enum class unusual_finish_reason {
        USER_GAVE_UP,
        USER_DISCONNECTED,
    };

    game(std::size_t id,
         std::string handle_p1,
         std::string handle_p2,
         std::function<void(const std::string &handle_winner, const std::string &handle_loser)>
             game_post_action) noexcept;

    void apply_command(const std::string &handle, std::unique_ptr<game_command> command);
    void update();
    void user_gave_up(const std::string &handle);
    void user_disconnected(const std::string &handle);
    void call_post_game_actions();

    [[nodiscard]] std::size_t get_id() const;
    [[nodiscard]] bool is_finished() const;
    [[nodiscard]] const std::string &get_handle_p1() const;
    [[nodiscard]] const std::string &get_handle_p2() const;

private:
    static void fill_body_with_snapshot(message<messages_type> &msg,
                                        const player_snapshot &p_snapshot) noexcept;
    void send_snapshots_lock_held() const;
    [[nodiscard]] std::pair<std::string, std::string> get_winner_loser_lock_held();

    std::size_t m_id;
    std::string m_handle_p1;
    std::string m_handle_p2;

    result m_result = result::PROCESSING;
    std::optional<unusual_finish_reason> m_finish_reason = std::nullopt;

    game_state m_state;
    std::function<void(const std::string &handle_winner, const std::string &handle_loser)> m_game_post_action;

    mutable std::mutex m_mutex;
};
}  // namespace war_of_ages::server

#endif  // WAR_OF_AGES_GAME_H
