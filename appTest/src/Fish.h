#pragma once

#include <SFML/Graphics.hpp>

class Fish {
public:
    Fish(const sf::Texture& texture, const std::vector<sf::FloatRect>& avoidZones);

    void draw(sf::RenderWindow& window) const;
    sf::Vector2f getPosition() const;
    float getRadius() const;
    bool isCollected() const;
    void markCollected();

private:
    sf::Sprite mSprite;
    bool mCollected;
};
