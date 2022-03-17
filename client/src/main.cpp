#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "../include/client.h"
#include "../include/screen_defines.h"
#include "../include/screens.h"
#include "../include/ui_functions.h"
#include "../../game_logic/include/age.h"
#include "../../game_logic/include/unit.h"
#include "../include/sprite_supplier.h"

namespace war_of_ages {

void screens_init(tgui::Gui &gui) {
    // Widgets of every screen are defined here

    // Ilya's screens

    tournament_screen_init(gui);
    tournament_creation_screen_init(gui);
    tournament_join_screen_init(gui);

    // end Ilya's screens

    // Vakhtang's screens

    start_screen_init(gui);
    multiplayer_screen_init(gui);
    settings_screen_init(gui);

    // end Vakhtang's screens
}

}  // namespace war_of_ages

int main() {
    sf::RenderWindow window{sf::VideoMode::getFullscreenModes()[0], "War of Ages", sf::Style::Fullscreen};
    tgui::Gui gui(window);

    war_of_ages::screens_init(gui);

    war_of_ages::client_state state("handle", war_of_ages::screen::START_SCREEN);

    gui.get(state.get_cur_screen_id())->setVisible(true);

    sf::Texture t;
    t.loadFromFile("../client/resources/pictures/fullHD_kittens.jpg");
    sf::Sprite s;
    s.setTexture(t);

    sf::Sprite tower_sprite = war_of_ages::sprite_supplier::get_instance().get_tower_sprite(age_type::STONE, war_of_ages::sprite_supplier::player_side::LEFT);
    sf::Sprite unit = war_of_ages::sprite_supplier::get_instance().get_unit_sprite(war_of_ages::unit_type::ARCHER, war_of_ages::sprite_supplier::player_side::RIGHT);

    unit.setPosition(500, 500);

    std::vector<sf::Sprite> units(50, unit);

    tower_sprite.setPosition(500, 500);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        war_of_ages::update_widgets(gui, state);

        window.clear();
        window.draw(s);

        gui.draw();

        window.draw(tower_sprite);

        for (auto &unit: units) {
            window.draw(unit);
        }
        for (auto &unit: units) {
            unit.setPosition((int)(unit.getPosition().x + (rand() % 2 ? -1 : 1) * rand() % 5 + window.getSize().x - 100) % (int)(window.getSize().x - 100), (int)(unit.getPosition().y + (rand() % 2 ? -1 : 1) * rand() % 5 + window.getSize().y - 100) % (int)(window.getSize().y - 100));
        }
        window.display();
    }
}