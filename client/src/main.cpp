#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Loading/ImageLoader.hpp>
#include <TGUI/TGUI.hpp>

/*
 *
 * JUST A SAMPLE CODE, WILL BE REMOVED
 *
 */

void bye() {
    exit(0);
}

int main() {
    sf::RenderWindow window{sf::VideoMode::getFullscreenModes()[0], "TGUI example - SFML_GRAPHICS backend", sf::Style::Fullscreen};
    tgui::Gui gui(window);

    sf::Texture t;
    t.loadFromFile("../client/resources/pictures/fullHD_kittens.jpg");
    sf::Sprite s;
    s.setTexture(t);

    tgui::Theme theme{"../client/resources/tgui_themes/Black.txt"};
    tgui::Button::Ptr exit_button = tgui::Button::create("__EXITEXITEXIT__");
    exit_button->setPosition("30%", "50%");
    exit_button->setSize("40%", "10%");
    exit_button->onPress(bye);
    exit_button->setRenderer(theme.getRenderer("Button"));
    gui.add(exit_button);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(s);

        gui.draw();

        window.display();
    }
}