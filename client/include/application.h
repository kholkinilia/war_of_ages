#ifndef WAR_OF_AGES_APPLICATION_H
#define WAR_OF_AGES_APPLICATION_H

namespace war_of_ages::client {

struct application {
    enum class state {
        MENU,
        SINGLE_PLAYER_GAME,
        MULTIPLAYER,
    };

    enum class multiplayer_handler {
        NONE,
        RANDOM,
        ROOM,
        TOURNAMENT,
    };

private:
    state m_state;
    multiplayer_handler m_multiplayer_handler = multiplayer_handler::NONE;
    void update_screens();

public:
    static void init();
    void run();
    void set_state(state new_state);

    [[nodiscard]] state get_state() const noexcept;
    [[nodiscard]] multiplayer_handler get_multiplayer_handler() const noexcept;

    [[nodiscard]] static application &instance();
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_APPLICATION_H
