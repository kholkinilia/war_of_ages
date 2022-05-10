#ifndef WAR_OF_AGES_SCREEN_HANDLER_H
#define WAR_OF_AGES_SCREEN_HANDLER_H

#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include <stack>
#include "../../common/game_logic/include/player.h"

namespace war_of_ages::client {

struct screen_handler {
    enum class screen_type {
        WAITING_FOR_SERVER,
        ROOM_SCREEN,
        TOURNAMENT_JOINING,
        TOURNAMENT_CREATION,
        TOURNAMENT_MAIN,
        START_SCREEN,
        SETTINGS,
        MULTIPLAYER,
        WAIT_OPPONENT,
        GAME_SCREEN,
        END_GAME,
        PREVIOUS,  // grandmaster move
        PREVIOUS_MENU,
    };
    const static inline std::map<screen_type, std::string> screen_id{
        {screen_type::ROOM_SCREEN, "room_screen"},
        {screen_type::WAITING_FOR_SERVER, "waiting_for_server"},
        {screen_type::TOURNAMENT_CREATION, "tournament_creation"},
        {screen_type::TOURNAMENT_JOINING, "tournament_joining"},
        {screen_type::TOURNAMENT_MAIN, "tournament_main"},
        {screen_type::START_SCREEN, "start_screen"},
        {screen_type::SETTINGS, "settings"},
        {screen_type::MULTIPLAYER, "multiplayer"},
        {screen_type::WAIT_OPPONENT, "wait_opponent"},
        {screen_type::GAME_SCREEN, "game_screen"},
        {screen_type::END_GAME, "end_game_screen"}};

private:
    tgui::Gui m_gui;
    std::stack<screen_type> m_screen_stack;

    static void place_widgets(std::vector<tgui::Widget::Ptr> &widgets,
                              int width = 40,
                              int height = 10,
                              int space = 5);

    void tournament_screen_init();
    void tournament_creation_screen_init();
    void tournament_join_screen_init();
    void start_screen_init();
    void multiplayer_screen_init();
    void settings_screen_init();
    void opponent_waiting_screen_init();
    void game_screen_init();
    void end_game_screen_init();
    void wait_for_server_screen_init();

    screen_handler();

public:
    void init(sf::RenderWindow &window);
    void change_screen(screen_type new_screen);

    [[nodiscard]] screen_type get_screen_type() const noexcept;
    [[nodiscard]] tgui::Gui &get_gui() noexcept;

    void update_fps();
    void draw();

    [[nodiscard]] static screen_handler &instance();
};

}  // namespace war_of_ages::client

#endif  // WAR_OF_AGES_SCREEN_HANDLER_H
