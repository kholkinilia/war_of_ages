#ifndef WAR_OF_AGES_SCREEN_DEFINES_H
#define WAR_OF_AGES_SCREEN_DEFINES_H

namespace war_of_ages {

enum class screen {
    TOURNAMENT_JOINING,
    TOURNAMENT_CREATION,
    TOURNAMENT_MAIN,
    START_SCREEN,
    SETTINGS,
    MULTIPLAYER
};

inline const std::map<screen, tgui::String> screen_id{{screen::TOURNAMENT_CREATION, "tournament_creation"},
                                                      {screen::TOURNAMENT_JOINING, "tournament_joining"},
                                                      {screen::TOURNAMENT_MAIN, "tournament_main"},
                                                      {screen::START_SCREEN, "start_screen"},
                                                      {screen::SETTINGS, "settings"},
                                                      {screen::MULTIPLAYER, "multiplayer"}};

}  // namespace war_of_ages
#endif  // WAR_OF_AGES_SCREEN_DEFINES_H
