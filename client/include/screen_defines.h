#ifndef WAR_OF_AGES_SCREEN_DEFINES_H
#define WAR_OF_AGES_SCREEN_DEFINES_H

namespace war_of_ages {

enum class screen { TOURNAMENT_JOINING, TOURNAMENT_CREATION, TOURNAMENT_MAIN };

inline const std::map<screen, tgui::String> screen_id{{screen::TOURNAMENT_CREATION, "tournament_creation"},
                                                      {screen::TOURNAMENT_JOINING, "tournament_joining"},
                                                      {screen::TOURNAMENT_MAIN, "tournament_main"}};

}  // namespace war_of_ages
#endif  // WAR_OF_AGES_SCREEN_DEFINES_H
