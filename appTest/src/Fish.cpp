#include "Fish.h"
#include <random>

Fish::Fish(const sf::Texture& texture, const std::vector<sf::FloatRect>& avoidZones)
    : mCollected(false)
{
    mSprite.setTexture(texture);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    mSprite.setScale(0.1f, 0.1f); // Redimensionner selon taille image

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, 1100.f);
    std::uniform_real_distribution<float> distY(100.f, 600.f);

    bool valid = false;
    while (!valid) {
        sf::Vector2f pos(distX(gen), distY(gen));
        mSprite.setPosition(pos);

        valid = true;
        for (const auto& zone : avoidZones) {
            if (zone.intersects(mSprite.getGlobalBounds())) {
                valid = false;
                break;
            }
        }
    }
}

void Fish::draw(sf::RenderWindow& window) const {
    if (!mCollected) {
        window.draw(mSprite);
    }
}

sf::Vector2f Fish::getPosition() const {
    return mSprite.getPosition();
}

float Fish::getRadius() const {
    return (mSprite.getGlobalBounds().width + mSprite.getGlobalBounds().height) / 4.f;
}

bool Fish::isCollected() const {
    return mCollected;
}

void Fish::markCollected() {
    mCollected = true;
}
