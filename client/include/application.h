#ifndef WAR_OF_AGES_APPLICATION_H
#define WAR_OF_AGES_APPLICATION_H

namespace war_of_ages {

struct application {
    enum class state {
        WAITING_FOR_SERVER,
        MENU,
        TOURNAMENT,
        ROOM,
        RANDOM_MATCHMAKING,
        SINGLE_PLAYER_GAME,
        MULTIPLAYER_GAME,
    };

private:
    state m_state;
    static void update_screens();

public:

    static void init();
    void run();
    void set_state(state new_state);

    [[nodiscard]] state get_state() const noexcept;

    [[nodiscard]] static application &instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_APPLICATION_H
