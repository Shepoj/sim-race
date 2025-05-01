#include "Obstacle.h"
#include <random>

/// Constructeur : génère un obstacle avec position et taille aléatoires
Obstacle::Obstacle() {
    std::random_device rd;
    std::mt19937 generateur(rd());

    // Distributions aléatoires pour la position et le rayon
    std::uniform_real_distribution<float> distributionX(100.f, 1100.f);
    std::uniform_real_distribution<float> distributionY(100.f, 600.f);
    std::uniform_real_distribution<float> distributionRayon(10.f, 40.f);

    float rayon = distributionRayon(generateur);
    forme.setRadius(rayon);
    forme.setFillColor(sf::Color(194, 178, 128)); // Couleur sable naturelle
    forme.setOrigin(rayon, rayon);               // Origine centrée pour un positionnement naturel
    forme.setPosition(distributionX(generateur), distributionY(generateur));
}

/// Affiche l'obstacle sur la fenêtre fournie
void Obstacle::draw(sf::RenderWindow& fenetre) const {
    fenetre.draw(forme);
}

/// Retourne la position centrale de l'obstacle
sf::Vector2f Obstacle::getPosition() const {
    return forme.getPosition();
}

/// Retourne le rayon de l'obstacle
float Obstacle::getRadius() const {
    return forme.getRadius();
}

/// Retourne les limites globales de l'obstacle (utile pour collisions ou positionnement)
sf::FloatRect Obstacle::getGlobalBounds() const {
    return forme.getGlobalBounds();
}
