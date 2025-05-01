#include "Fish.h"
#include <random>

/// Constructeur : initialise le sprite et le place dans une position valide
Fish::Fish(const sf::Texture& texture, const std::vector<sf::FloatRect>& avoidZones)
    : mCollected(false)
{
    // Appliquer la texture
    mSprite.setTexture(texture);

    // Centrer l'origine du sprite
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Réduction d'échelle (taille du poisson)
    mSprite.setScale(0.1f, 0.1f);

    // Générateur aléatoire
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, 1100.f);
    std::uniform_real_distribution<float> distY(100.f, 600.f);

    // Trouver une position non chevauchante avec les zones interdites
    bool valide = false;
    while (!valide) {
        sf::Vector2f position(distX(gen), distY(gen));
        mSprite.setPosition(position);

        valide = true;
        for (const auto& zone : avoidZones) {
            if (zone.intersects(mSprite.getGlobalBounds())) {
                valide = false;
                break;
            }
        }
    }
}

/// Dessine le poisson seulement s’il n’a pas été ramassé
void Fish::draw(sf::RenderWindow& window) const {
    if (!mCollected) {
        window.draw(mSprite);
    }
}

/// Retourne la position actuelle du poisson
sf::Vector2f Fish::getPosition() const {
    return mSprite.getPosition();
}

/// Retourne un rayon estimé (utilisé pour la détection de proximité)
float Fish::getRadius() const {
    return (mSprite.getGlobalBounds().width + mSprite.getGlobalBounds().height) / 4.f;
}

/// Vérifie si le poisson a déjà été ramassé
bool Fish::isCollected() const {
    return mCollected;
}

/// Marque le poisson comme ramassé
void Fish::markCollected() {
    mCollected = true;
}
