#pragma once

#include <SFML/Graphics.hpp>

class Obstacle {
public:
    Obstacle();
    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    float getRadius() const;


private:
    sf::CircleShape mShape;
};
