#include "Port.h"
#include <string>

Port::Port(const sf::Vector2f& position, const sf::Color& color, const sf::Font& font)
    : mScore(0)
{
    mShape.setSize(sf::Vector2f(40.f, 120.f));
    mShape.setOrigin(20.f, 60.f);
    mShape.setPosition(position);
    mShape.setFillColor(color);

    mScoreText.setFont(font);
    mScoreText.setCharacterSize(20);
    mScoreText.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = mScoreText.getLocalBounds();
    mScoreText.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);

// Position centrale du port
mScoreText.setPosition(position.x- 6.f, position.y - 10.f); 
    mScoreText.setString("0");
}

void Port::draw(sf::RenderWindow& window) const {
    window.draw(mShape);
    window.draw(mScoreText);

}

sf::FloatRect Port::getBounds() const {
    return mShape.getGlobalBounds();
}

void Port::depositFish() {
    ++mScore;
    mScoreText.setString(std::to_string(mScore));
}

int Port::getScore() const {
    return mScore;
}
