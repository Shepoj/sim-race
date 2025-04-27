#include "Menu.h"

void Menu::draw(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text title("Course de Bateaux", font, 40);
    title.setPosition(300.f, 100.f);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    sf::Text options("1 joueur : Appuie sur 1\n2 joueurs : Appuie sur 2", font, 30);
    options.setPosition(300.f, 250.f);
    options.setFillColor(sf::Color::Black);
    window.draw(options);
}
