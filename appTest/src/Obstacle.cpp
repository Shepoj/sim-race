#include "Obstacle.h"
#include <random>

Obstacle::Obstacle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, 1100.f);
    std::uniform_real_distribution<float> distY(100.f, 600.f);
    std::uniform_real_distribution<float> distR(10.f, 40.f);

    float radius = distR(gen);
    mShape.setRadius(radius);
    mShape.setFillColor(sf::Color::Blue);
    mShape.setOrigin(radius, radius);
    mShape.setPosition(distX(gen), distY(gen));
}

void Obstacle::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
}

sf::Vector2f Obstacle::getPosition() const {
    return mShape.getPosition();
}

float Obstacle::getRadius() const {
    return mShape.getRadius();
}

sf::FloatRect Obstacle::getGlobalBounds() const {
    return mShape.getGlobalBounds(); // si tu utilises un sf::CircleShape mShape
}
