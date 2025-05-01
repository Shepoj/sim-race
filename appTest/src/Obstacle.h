#pragma once

#include <SFML/Graphics.hpp>

/// Classe représentant un obstacle circulaire dans le jeu
class Obstacle {
public:
    /// Constructeur aléatoire : positionne et dimensionne l'obstacle
    Obstacle();

    /// Affiche l'obstacle dans la fenêtre
    void draw(sf::RenderWindow& fenetre) const;

    /// Retourne la position centrale de l'obstacle
    sf::Vector2f getPosition() const;

    /// Retourne le rayon de l'obstacle
    float getRadius() const;

    /// Retourne les limites globales de l'obstacle (utilisé pour éviter les chevauchements)
    sf::FloatRect getGlobalBounds() const;

private:
    sf::CircleShape forme; // Forme circulaire représentant l'obstacle
};
