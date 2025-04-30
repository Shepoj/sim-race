#pragma once

#include <SFML/Graphics.hpp>

class Port {
public:
    Port(const sf::Vector2f& position, const sf::Color& color, const sf::Font& font);

    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;
    void depositFish();
    int getScore() const;

private:
    sf::RectangleShape mShape;
    int mScore;
    sf::Text mScoreText;
};
