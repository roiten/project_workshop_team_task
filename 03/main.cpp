#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

void pollEvents(sf::RenderWindow &window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            default:
                break;
        }
    }
}

void redrawFrame(sf::RenderWindow &window, sf::Sprite &cat) {
    window.clear(sf::Color::White);
    window.draw(cat);
    window.display();
}

int main() {
    constexpr unsigned WINDOW_WIDTH = 800;
    constexpr unsigned WINDOW_HEIGHT = 600;
    constexpr unsigned ANTI_ALIASING = 8;
    const std::string CAT_IMAGE = "cat.png";

    sf::ContextSettings settings;
    settings.antialiasingLevel = ANTI_ALIASING;
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}),
        "Sprite showing", sf::Style::Default, settings);

    sf::Texture texture;
    texture.loadFromFile(CAT_IMAGE);
    sf::Sprite cat(texture);

    while (window.isOpen()) {
        pollEvents(window);
        redrawFrame(window, cat);
    }
}
