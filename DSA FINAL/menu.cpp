#include "menu.hpp"

Menu::Menu(RenderWindow* window) : window(window) {}

MenuOptions Menu::runMainMenu() {
    Font font;
    if (!font.loadFromFile("assets/arial.ttf")) {
        // fallback or handle error
    }

    Text title("Xonix", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(100, 50);

    Text playOption("Play", font, 30);
    playOption.setPosition(150, 150);

    Text exitOption("Exit", font, 30);
    exitOption.setPosition(150, 220);

    int selected = 0; // 0 = Play, 1 = Exit

    while (window->isOpen()) {
        Event e;
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed)
                window->close();

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up || e.key.code == Keyboard::W)
                    selected = (selected == 0) ? 1 : 0; // wrap around
                if (e.key.code == Keyboard::Down || e.key.code == Keyboard::S)
                    selected = (selected == 1) ? 0 : 1;

                if (e.key.code == Keyboard::Enter || e.key.code == Keyboard::Space) {
                    return (selected == 0) ? PLAY : EXIT;
                }
            }
        }

        // Highlight selection
        playOption.setFillColor(selected == 0 ? sf::Color::Yellow : sf::Color::White);
        exitOption.setFillColor(selected == 1 ? sf::Color::Yellow : sf::Color::White);

        // draw menu
        window->clear(sf::Color::Black);
        window->draw(title);
        window->draw(playOption);
        window->draw(exitOption);
        window->display();
    }

    return EXIT; // if window is closed
}