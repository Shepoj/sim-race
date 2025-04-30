#include "Menu.h"

void Menu::draw(sf::RenderWindow& window, const sf::Font& font) {
    sf::Text title("Course de Bateaux", font, 40);
    sf::FloatRect tb = title.getLocalBounds();
    title.setOrigin(tb.width / 2.f, tb.height / 2.f);
    title.setPosition(640.f, 200.f); // centré horizontalement
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    sf::Text options("1 joueur : Appuyer sur 1\n2 joueurs : Appuyer sur 2", font, 30);
    sf::FloatRect ib = options.getLocalBounds();
    options.setOrigin(ib.width / 2.f, ib.height / 2.f);
    options.setPosition(640.f, 320.f);
    options.setFillColor(sf::Color::Black);
    window.draw(options);
}