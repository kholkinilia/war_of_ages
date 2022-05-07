#ifndef WAR_OF_AGES_SCREEN_HANDLER_H
#define WAR_OF_AGES_SCREEN_HANDLER_H

#include <TGUI/Backend/SFML-Graphics.hpp>  // tgui::Gui
#include "client.h"
#include "../../common/game_logic/include/player.h"

namespace war_of_ages {

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
    };
    const static inline std::map<screen_type, std::string> screen_id{
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
    screen_type m_screen_type = screen_type::START_SCREEN;

    screen_handler() = default;
    void set_window(sf::RenderWindow &window);

public:
    void init(sf::RenderWindow &window);
    void change_screen(screen_type new_screen);

    void update_fps();

    [[nodiscard]] screen_type get_screen_type() const noexcept;
    [[nodiscard]] tgui::Gui &get_gui() noexcept;

    [[nodiscard]] static screen_handler &instance();
};

}  // namespace war_of_ages

#endif  // WAR_OF_AGES_SCREEN_HANDLER_H
